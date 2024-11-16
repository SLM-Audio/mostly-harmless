//
// Created by Syl Morrison on 20/10/2024.
//

#ifndef MOSTLYHARMLESS_MOSTLYHARMLESS_IENGINE_H
#define MOSTLYHARMLESS_MOSTLYHARMLESS_IENGINE_H
#include <mostly_harmless/core/mostlyharmless_ISharedState.h>
#include <mostly_harmless/mostlyharmless_TransportState.h>
#include <mostly_harmless/mostlyharmless_Concepts.h>
#include <marvin/containers/marvin_BufferView.h>
#include <marvin/library/marvin_Concepts.h>
namespace mostly_harmless::core {

    struct InitContext {
        double sampleRate;
        std::uint32_t minBufferSize;
        std::uint32_t maxBufferSize;
    };

    struct ProcessContext {
        marvin::containers::BufferView<float> buffer;
        std::optional<mostly_harmless::TransportState> transportState;
    };

    class IEngine {
    public:
        virtual ~IEngine() noexcept = default;
        virtual void initialise(InitContext context) noexcept = 0;
        virtual void process(ProcessContext context) noexcept = 0;
        virtual void reset() noexcept = 0;
        virtual void handleNoteOn([[maybe_unused]] std::uint16_t portIndex, [[maybe_unused]] std::uint8_t channel, [[maybe_unused]] std::uint8_t note, [[maybe_unused]] double velocity) {}
        virtual void handleNoteOff([[maybe_unused]] std::uint16_t portIndex, [[maybe_unused]] std::uint8_t channel, [[maybe_unused]] std::uint8_t note, [[maybe_unused]] double velocity) {}
    };
} // namespace mostly_harmless::core
#endif // MOSTLYHARMLESS_MOSTLYHARMLESS_IENGINE_H
