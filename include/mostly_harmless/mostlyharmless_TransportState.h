//
// Created by Syl Morrison on 25/09/2024.
//

#ifndef MOSTLYHARMLESS_MOSTLYHARMLESS_TRANSPORTSTATE_H
#define MOSTLYHARMLESS_MOSTLYHARMLESS_TRANSPORTSTATE_H
#include <clap/events.h>
#include <optional>
namespace mostly_harmless {

    /**
     * \brief Convenience struct holding the numerator and denominator of a time signature. Two ints in a trenchcoat.
     *
     */
    struct TimeSignature {
        /**
         * The numerator of the time signature (the "4" in 4/4)
         */
        std::uint16_t numerator;
        /**
         * The denominator of the time signature (the other "4" in 4/4)
         */
        std::uint16_t denominator;
    };

    /**
     * \brief Convenience struct holding relevant positional info about a loop the host may currently be running.
     */
    struct LoopInfo {
        /**
         * The position of the start of the loop, in beats.
         */
        double startBeats;
        /**
         * The position of the end of the loop, in beats.
         */
        double endBeats;
        /**
         * The position of the start of the loop, in seconds.
         */
        double startSeconds;
        /**
         * The position of the end of the loop, in seconds.
         */
        double endSeconds;
    };

    /**
     * \brief Contains info from the host about the current transport state.
     *
     * Note that the host isn't guaranteed to provide all of, or even any of this information - which is why most of the members are optional.
     * Make sure you check they're valid before blindly dereferencing them!
     */
    struct TransportState final {
        /**
         * @private
         */
        explicit TransportState(const clap_event_transport_t* transportEvent);

        /**
         * Contains the current bpm of the host, or std::nullopt.
         */
        std::optional<double> bpm{};
        /**
         * Contains the current playback position in beats, or std::nullopt.
         */
        std::optional<double> positionBeats{};
        /**
         * Contains the current playback position in seconds, or std::nullopt.
         */
        std::optional<double> positionSeconds{};
        /**
         * Contains the current time signature, or std::nullopt.
         */
        std::optional<TimeSignature> timeSignature{};
        /**
         * true if the host is currently playing, false otherwise.
         */
        bool isPlaying{ false };
        /**
         * true if the host is currently recording, false otherwise.
         */
        bool isRecording{ false };
        /**
         * true if the host is currently looping, false otherwise. If this is true, then (hopefully), loopInfo is not a nullopt..
         */
        bool isLoopActive{ false };
        /**
         * Contains positional info about the currently active loop (if isLoopActive), or nullopt.
         */
        std::optional<LoopInfo> loopInfo;
        /**
         * true if the host is currently in a "preroll" (count-in).
         */
        bool isWithinPreroll{ false };
    };

} // namespace mostly_harmless
#endif // MOSTLYHARMLESS_MOSTLYHARMLESS_TRANSPORTSTATE_H
