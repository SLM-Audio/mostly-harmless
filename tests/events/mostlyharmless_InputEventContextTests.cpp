//
// Created by Syl Morrison on 05/10/2024.
//
#include <catch2/catch_test_macros.hpp>
#include <mostly_harmless/events/mostlyharmless_InputEventContext.h>
#include <clap/helpers/event-list.hh>
namespace mostly_harmless::testing {
    clap_event_note createNoteEvent(int noteId, std::uint32_t time, std::int16_t midiKey, bool on, clap::helpers::EventList& eventList) {
        clap_event_note noteEvent;
        noteEvent.header.type = on ? static_cast<std::uint16_t>(CLAP_EVENT_NOTE_ON) : static_cast<std::uint16_t>(CLAP_EVENT_NOTE_OFF);
        noteEvent.header.size = sizeof(clap_event_note);
        noteEvent.header.time = time;
        noteEvent.header.space_id = CLAP_CORE_EVENT_SPACE_ID;
        noteEvent.header.flags = 0;
        noteEvent.channel = 0;
        noteEvent.key = midiKey;
        noteEvent.velocity = 1.0;
        noteEvent.port_index = 0;
        noteEvent.note_id = noteId;
        eventList.push(&noteEvent.header);
        return noteEvent;
    }


    TEST_CASE("Test InputEventContext") {
        constexpr static size_t numSamples{ 60 };
        clap::helpers::EventList eventList;
        std::vector<clap_event_note> expectedEvents;
        expectedEvents.emplace_back(createNoteEvent(1, 2, 60, true, eventList));
        expectedEvents.emplace_back(createNoteEvent(2, 4, 30, true, eventList));
        expectedEvents.emplace_back(createNoteEvent(1, 14, 60, false, eventList));
        expectedEvents.emplace_back(createNoteEvent(2, 16, 30, false, eventList));
        events::InputEventContext context{ eventList.clapInputEvents() };
        size_t indexInEventList{ 0 };
        for (size_t i = 0; i < numSamples; ++i) {
            while (context.next() && context.next()->time == i) {
                auto* asNoteEvent = reinterpret_cast<const clap_event_note*>(context.next());
                auto& correspondingEvent = expectedEvents[indexInEventList];
                REQUIRE(asNoteEvent->note_id == correspondingEvent.note_id);
                REQUIRE(asNoteEvent->key == correspondingEvent.key);
                REQUIRE(asNoteEvent->velocity == correspondingEvent.velocity);
                REQUIRE(asNoteEvent->channel == correspondingEvent.channel);
                REQUIRE(asNoteEvent->header.type == correspondingEvent.header.type);
                ++indexInEventList;
                ++context;
            }
        }
    }


} // namespace mostly_harmless::testing