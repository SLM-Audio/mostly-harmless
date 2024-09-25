//
// Created by Syl Morrison on 25/09/2024.
//
#include <mostly_harmless/mostlyharmless_TransportState.h>
namespace mostly_harmless {
    TransportState::TransportState(const clap_event_transport_t* transportEvent) {
        const auto flags = transportEvent->flags;
        if (const auto hasBpm = flags & CLAP_TRANSPORT_HAS_TEMPO; hasBpm) {
            bpm = transportEvent->tempo;
        }
        if (const auto hasTimeBeats = flags & CLAP_TRANSPORT_HAS_BEATS_TIMELINE; hasTimeBeats) {
            positionBeats = transportEvent->song_pos_beats;
        }
        if (const auto hasTimeSeconds = flags & CLAP_TRANSPORT_HAS_SECONDS_TIMELINE; hasTimeSeconds) {
            positionSeconds = transportEvent->song_pos_seconds;
        }
        if (const auto hasTimeSignature = flags & CLAP_TRANSPORT_HAS_TIME_SIGNATURE; hasTimeSignature) {
            timeSignature = { .numerator = transportEvent->tsig_num, .denominator = transportEvent->tsig_denom };
        }
        isPlaying = flags & CLAP_TRANSPORT_IS_PLAYING;
        isRecording = flags & CLAP_TRANSPORT_IS_RECORDING;
        if (isLoopActive = flags & CLAP_TRANSPORT_IS_LOOP_ACTIVE; isLoopActive) {
            loopInfo = {
                .loopStartBeats = transportEvent->loop_start_beats,
                .loopEndBeats = transportEvent->loop_end_beats,
                .loopStartSeconds = transportEvent->loop_start_seconds,
                .loopEndSeconds = transportEvent->loop_end_seconds
            };
        }
        isWithinPreroll = flags & CLAP_TRANSPORT_IS_WITHIN_PRE_ROLL;
    }
} // namespace mostly_harmless