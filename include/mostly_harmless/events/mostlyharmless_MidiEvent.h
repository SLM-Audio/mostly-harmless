#ifndef MOSTLY_HARMLESS_MIDI_EVENT
#define MOSTLY_HARMLESS_MIDI_EVENT
#include <variant>
#include <optional>
/// @internal
namespace mostly_harmless::events::midi {
    /// @internal
    struct NoteOn final {
        std::uint8_t channel;
        std::uint8_t note;
        double velocity;
    };

    /// @internal
    struct NoteOff final {
        std::uint8_t channel;
        std::uint8_t note;
        double velocity;
    };

    /// @internal
    struct PolyAftertouch final {
        std::uint8_t channel;
        std::uint8_t note;
        std::uint8_t pressure;
    };

    /// @internal
    struct ControlChange final {
        std::uint8_t channel;
        std::uint8_t controllerNumber;
        std::uint8_t data;
    };

    /// @internal
    struct ProgramChange final {
        std::uint8_t channel;
        std::uint8_t programNumber;
    };

    /// @internal
    struct ChannelAftertouch final {
        std::uint8_t channel;
        std::uint8_t pressure;
    };

    /// @internal
    struct PitchWheel final {
        std::uint8_t channel;
        double value;
    };

    /// @internal
    using MidiEvent = std::variant<NoteOn, NoteOff, PolyAftertouch, ControlChange, ProgramChange, ChannelAftertouch, PitchWheel>;

    /// @internal
    auto parse(std::uint8_t b0, std::uint8_t b1, std::uint8_t b2) -> std::optional<MidiEvent>;
} // namespace mostly_harmless::events::midi

#endif