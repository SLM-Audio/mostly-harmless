#include <mostly_harmless/events/mostlyharmless_MidiEvent.h>
namespace mostly_harmless::events::midi {
    constexpr static auto s_note_off{ 0x80 };
    constexpr static auto s_note_on{ 0x90 };
    constexpr static auto s_poly_aftertouch{ 0xA0 };
    constexpr static auto s_control_change{ 0xB0 };
    constexpr static auto s_program_change{ 0xC0 };
    constexpr static auto s_channel_aftertouch{ 0xD0 };
    constexpr static auto s_pitch_wheel{ 0xE0 };

    auto parse(std::uint8_t b0, std::uint8_t b1, std::uint8_t b2) -> std::optional<MidiEvent> {
        const std::uint8_t message = b0 & 0xF0;
        const std::uint8_t channel = b0 & 0x0F;
        switch (message) {
            case s_note_on: [[fallthrough]];
            case s_note_off: {
                const std::uint8_t note = b1;
                const std::uint8_t velocity = b2;
                const auto floatVel = static_cast<double>(velocity) / 127.0;
                if (message == s_note_on && velocity != 0) {
                    return NoteOn{ .channel = channel, .note = note, .velocity = floatVel };
                }
                return NoteOff{ .channel = channel, .note = note, .velocity = floatVel };
            }
            case s_poly_aftertouch: return PolyAftertouch{ .channel = channel, .note = b1, .pressure = b2 };
            case s_control_change: return ControlChange{ .channel = channel, .controllerNumber = b1, .data = b2 };
            case s_program_change: return ProgramChange{ .channel = channel, .programNumber = b1 };
            case s_channel_aftertouch: return ChannelAftertouch{ .channel = channel, .pressure = b1 };
            case s_pitch_wheel: {
                const std::int16_t combined = b1 | (b2 << 7);
                double res = static_cast<double>(combined - 8192) / 8192.0;
                return PitchWheel{ .channel = channel, .value = res };
            }
            default: return {};
        }
    }

} // namespace mostly_harmless::events::midi