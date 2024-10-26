//
// Created by Syl on 19/08/2024.
//

#ifndef MOSTLYHARMLESS_MOSTLYHARMLESS_AUDIOHELPERS_H
#define MOSTLYHARMLESS_MOSTLYHARMLESS_AUDIOHELPERS_H
#include "mostly_harmless/events/mostlyharmless_InputEventContext.h"
#include "marvin/containers/marvin_BufferView.h"
#include <functional>
namespace mostly_harmless {
    /**
     * \brief Splits an input buffer into chunks and dispatches them, allowing for block based processing.
     *
     * As parameter and note events are sample accurate, and passed via a single queue, naively doing block based processing with the entire
     * input buffer would mean only having one param / note update event per block. To get around this, this function handles chunking the audio
     * between events, and calling some process function with that chunk.
     * For example:
     * ```cpp
     * void Plugin::process(marvin::containers::ByfferView<float> buffer, mostly_harmless::events::InputEventContext context) {
     *     runBlockDispatch(buffer, context,
     *         [this](const clap_event_header* event) -> void {
     *             handleEvent(event);
     *         },
     *         [this](marvin::containers::BufferView<float> chunk) -> void {
     *             // assuming a member called m_processor
     *             m_processor.process(chunk);
     *         }
     *     );
     * }
     * ```
     * \param buffer The entire buffer for this block
     * \param eventCallback The input event queue for this block.
     * \param eventCallback A callback to invoke on a new event.
     * \param blockCallback A callback to invoke on a new chunk.
     */
    template <marvin::FloatType SampleType>
    void runBlockDispatch(marvin::containers::BufferView<SampleType> buffer,
                          mostly_harmless::events::InputEventContext eventContext,
                          std::function<void(const clap_event_header* event)>&& eventCallback,
                          std::function<void(marvin::containers::BufferView<SampleType>)>&& blockCallback) {
        const auto numChannels = buffer.getNumChannels();
        auto** channelArray = static_cast<SampleType**>(alloca(sizeof(SampleType*) * numChannels));
        size_t lastEventIndex{ 0 };
        auto* const* rawBuff = buffer.getArrayOfWritePointers();
        for (size_t i = 0; i < buffer.getNumSamples(); ++i) {
            if (eventContext.next() && eventContext.next()->time == static_cast<std::uint32_t>(i)) {
                while (eventContext.next() && eventContext.next()->time == static_cast<std::uint32_t>(i)) {
                    eventCallback(eventContext.next());
                    ++eventContext;
                }
                for (size_t channel = 0; channel < numChannels; ++channel) {
                    auto* offsetChannelPtr = rawBuff[channel] + static_cast<std::ptrdiff_t>(lastEventIndex);
                    channelArray[channel] = offsetChannelPtr;
                }
                const auto numSamples = i - lastEventIndex;
                marvin::containers::BufferView<SampleType> slice{ channelArray, buffer.getNumChannels(), numSamples };
                blockCallback(slice);
                lastEventIndex = i;
            }
        }
        const auto remaining = static_cast<std::int64_t>(buffer.getNumSamples()) - static_cast<std::int64_t>(lastEventIndex);
        if (remaining <= 0) return;
        for (size_t channel = 0; channel < numChannels; ++channel) {
            auto* offsetChannelPtr = rawBuff[channel] + static_cast<std::ptrdiff_t>(lastEventIndex);
            channelArray[channel] = offsetChannelPtr;
        }
        marvin::containers::BufferView<SampleType> slice{ channelArray, buffer.getNumChannels(), static_cast<size_t>(remaining) };
        blockCallback(slice);
    }
} // namespace mostly_harmless

#endif // MOSTLYHARMLESS_MOSTLYHARMLESS_AUDIOHELPERS_H
