//
// Created by Syl Morrison on 25/09/2024.
//
#include <mostly_harmless/mostlyharmless_TransportState.h>
namespace mostly_harmless {
    [[nodiscard]] static double beattimeToDouble(clap_beattime t) {
        return static_cast<double>(t) / static_cast<double>(CLAP_BEATTIME_FACTOR);
    }

    [[nodiscard]] static double sectimeToDouble(clap_sectime t) {
        return static_cast<double>(t) / static_cast<double>(CLAP_SECTIME_FACTOR);
    }

    TransportState::TransportState(const clap_event_transport_t* transportEvent) {
        const auto flags = transportEvent->flags;
        if (const auto hasBpm = flags & CLAP_TRANSPORT_HAS_TEMPO; hasBpm) {
            bpm = transportEvent->tempo;
        }
        if (auto hasTimeBeats = flags & CLAP_TRANSPORT_HAS_BEATS_TIMELINE) {
            positionBeats = beattimeToDouble(transportEvent->song_pos_beats);
        }
        if (auto hasTimeSeconds = flags & CLAP_TRANSPORT_HAS_SECONDS_TIMELINE) {
            positionSeconds = sectimeToDouble(transportEvent->song_pos_seconds);
        }
        if (const auto hasTimeSignature = flags & CLAP_TRANSPORT_HAS_TIME_SIGNATURE; hasTimeSignature) {
            timeSignature = { .numerator = transportEvent->tsig_num, .denominator = transportEvent->tsig_denom };
        }
        isPlaying = flags & CLAP_TRANSPORT_IS_PLAYING;
        isRecording = flags & CLAP_TRANSPORT_IS_RECORDING;
        if (isLoopActive = flags & CLAP_TRANSPORT_IS_LOOP_ACTIVE; isLoopActive) {
            loopInfo = {
                .startBeats = beattimeToDouble(transportEvent->loop_start_beats),
                .endBeats = beattimeToDouble(transportEvent->loop_end_beats),
                .startSeconds = sectimeToDouble(transportEvent->loop_start_seconds),
                .endSeconds = sectimeToDouble(transportEvent->loop_end_seconds)
            };
        }
        isWithinPreroll = flags & CLAP_TRANSPORT_IS_WITHIN_PRE_ROLL;
    }
} // namespace mostly_harmless