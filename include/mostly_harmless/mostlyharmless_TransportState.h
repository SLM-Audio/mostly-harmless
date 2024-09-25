//
// Created by Syl Morrison on 25/09/2024.
//

#ifndef MOSTLYHARMLESS_MOSTLYHARMLESS_TRANSPORTSTATE_H
#define MOSTLYHARMLESS_MOSTLYHARMLESS_TRANSPORTSTATE_H
#include <clap/events.h>
#include <optional>
namespace mostly_harmless {

    struct TimeSignature {
        std::uint16_t numerator;
        std::uint16_t denominator;
    };

    struct LoopInfo {
        std::int64_t loopStartBeats;
        std::int64_t loopEndBeats;
        std::int64_t loopStartSeconds;
        std::int64_t loopEndSeconds;
    };

    struct TransportState final {
        explicit TransportState(const clap_event_transport_t* transportEvent);

        std::optional<double> bpm{};
        std::optional<std::int64_t> positionBeats{};
        std::optional<std::int64_t> positionSeconds{};
        std::optional<TimeSignature> timeSignature{};
        bool isPlaying;
        bool isRecording;
        bool isLoopActive;
        std::optional<LoopInfo> loopInfo;
        bool isWithinPreroll;
    };

} // namespace mostly_harmless
#endif // MOSTLYHARMLESS_MOSTLYHARMLESS_TRANSPORTSTATE_H
