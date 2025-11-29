//
// Created by Syl Morrison on 29/11/2025.
//
#include <mostly_harmless/events/mostlyharmless_MidiEvent.h>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
namespace mostly_harmless::testing {
    struct MidiMessage {
        std::uint8_t b0;
        std::uint8_t b1;
        std::uint8_t b2;
    };
    template <typename Expected>
    static auto check_result(std::optional<events::midi::MidiEvent> to_check) -> void {
        REQUIRE(to_check);
        REQUIRE(std::holds_alternative<Expected>(*to_check));
    }
    TEST_CASE("Test Midi Status Bytes") {
        SECTION("Test NoteOff") {
            const MidiMessage msg{ 0b10000000, 36, 0 };
            const auto res_opt = events::midi::parse(msg.b0, msg.b1, msg.b2);
            check_result<events::midi::NoteOff>(res_opt);
            const auto [channel, note, velocity] = std::get<events::midi::NoteOff>(*res_opt);
            REQUIRE(channel == 0);
            REQUIRE(note == 36);
            REQUIRE_THAT(velocity, Catch::Matchers::WithinRel(0.0));
        }
        SECTION("Test NoteOn") {
            const MidiMessage msg{ 0b10010000, 36, 127 };
            const auto res_opt = events::midi::parse(msg.b0, msg.b1, msg.b2);
            check_result<events::midi::NoteOn>(res_opt);
            const auto [channel, note, velocity] = std::get<events::midi::NoteOn>(*res_opt);
            REQUIRE(channel == 0);
            REQUIRE(note == 36);
            REQUIRE_THAT(velocity, Catch::Matchers::WithinRel(1.0f));
        }
        SECTION("Test PolyAftertouch") {
            const MidiMessage msg{ 0b10100001, 60, 127 };
            const auto res_opt = events::midi::parse(msg.b0, msg.b1, msg.b2);
            check_result<events::midi::PolyAftertouch>(res_opt);
            const auto [channel, note, pressure] = std::get<events::midi::PolyAftertouch>(*res_opt);
            REQUIRE(channel == 1);
            REQUIRE(note == 60);
            REQUIRE(pressure == 127);
        }
        SECTION("Test ControlChange") {
            const MidiMessage msg{ 0b10110001, 0, 127 };
            const auto res_opt = events::midi::parse(msg.b0, msg.b1, msg.b2);
            check_result<events::midi::ControlChange>(res_opt);
            const auto [channel, controllerNumber, value] = std::get<events::midi::ControlChange>(*res_opt);
            REQUIRE(channel == 1);
            REQUIRE(controllerNumber == 0);
            REQUIRE(value == 127);
        }
        SECTION("Test ProgramChange") {
            const MidiMessage msg{ 0b11000000, 2, 0 };
            const auto res_opt = events::midi::parse(msg.b0, msg.b1, msg.b2);
            check_result<events::midi::ProgramChange>(res_opt);
            const auto [channel, program] = std::get<events::midi::ProgramChange>(*res_opt);
            REQUIRE(channel == 0);
            REQUIRE(program == 2);
        }
        SECTION("Test ChannelAftertouch") {
            const MidiMessage msg{ 0b11010010, 8, 0 };
            const auto res_opt = events::midi::parse(msg.b0, msg.b1, msg.b2);
            check_result<events::midi::ChannelAftertouch>(res_opt);
            const auto [channel, pressure] = std::get<events::midi::ChannelAftertouch>(*res_opt);
            REQUIRE(channel == 2);
            REQUIRE(pressure == 8);
        }
        SECTION("Test PitchWheel") {
            const MidiMessage msg{ 0b11100000, 0x00, 0x40 };
            const auto res_opt = events::midi::parse(msg.b0, msg.b1, msg.b2);
            check_result<events::midi::PitchWheel>(res_opt);
            const auto [channel, value] = std::get<events::midi::PitchWheel>(*res_opt);
            REQUIRE(channel == 0);
            REQUIRE_THAT(value, Catch::Matchers::WithinRel(0.0));
        }

        SECTION("Test 0 Velocity Note-On") {
            const MidiMessage msg{ 0b10010000, 40, 0 };
            const auto res_opt = events::midi::parse(msg.b0, msg.b1, msg.b2);
            check_result<events::midi::NoteOff>(res_opt);
            const auto [channel, note, velocity] = std::get<events::midi::NoteOff>(*res_opt);
            REQUIRE(channel == 0);
            REQUIRE(note == 40);
            REQUIRE_THAT(velocity, Catch::Matchers::WithinRel(0.0));
        }
    }

} // namespace mostly_harmless::testing
