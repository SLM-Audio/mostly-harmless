//
// Created by Syl on 19/08/2024.
//

#ifndef MOSTLYHARMLESS_MOSTLYHARMLESS_AUDIOHELPERS_H
#define MOSTLYHARMLESS_MOSTLYHARMLESS_AUDIOHELPERS_H
#include "mostly_harmless/mostlyharmless_Plugin.h"
#include "mostly_harmless/events/mostlyharmless_InputEventContext.h"
#include "marvin/containers/marvin_BufferView.h"
#include <functional>
namespace mostly_harmless {
    template <marvin::FloatType SampleType>
    void runBlockDispatch(marvin::containers::BufferView<SampleType> buffer,
                          mostly_harmless::events::InputEventContext eventContext,
                          std::function<void(const clap_event_header* event)>&& eventCallback,
                          std::function<void(marvin::containers::BufferView<SampleType>)>&& blockCallback) {
        size_t lastEventIndex{ 0 };
        auto* const* rawBuff = buffer.getArrayOfWritePointers();
        for (auto i = 0; i < buffer.getNumSamples(); ++i) {
            if (eventContext.next() && eventContext.next()->time == i) {
                while (eventContext.next() && eventContext.next()->time == i) {
                    eventCallback(eventContext.next());
                }
                const auto numSamples = i - lastEventIndex;
                marvin::containers::BufferView<SampleType> slice{ rawBuff + static_cast<std::ptrdiff_t>(lastEventIndex), buffer.getNumChannels(), numSamples };
                blockCallback(slice);
                lastEventIndex = i;
            }
        }
        const auto remaining = static_cast<std::int64_t>(buffer.getNumSamples()) - static_cast<std::int64_t>(lastEventIndex);
        if (remaining <= 0) return;
        marvin::containers::BufferView<SampleType> slice{ rawBuff + static_cast<std::ptrdiff_t>(lastEventIndex), buffer.getNumChannels(), static_cast<size_t>(remaining) };
        blockCallback(slice);
    }
} // namespace mostly_harmless

#endif // MOSTLYHARMLESS_MOSTLYHARMLESS_AUDIOHELPERS_H
