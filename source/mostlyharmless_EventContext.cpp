//
// Created by Syl on 11/08/2024.
//
#include <mostly_harmless/mostlyharmless_EventContext.h>
namespace mostly_harmless {
    EventContext::EventContext(const clap_input_events_t* inputEventQueue) : m_inputEventQueue(inputEventQueue) {
        m_inputEventQueueSize = m_inputEventQueue->size(m_inputEventQueue);
        if (m_inputEventQueueSize == 0) return;
        m_nextEvent = m_inputEventQueue->get(m_inputEventQueue, m_nextEventIndex);
    }

    EventContext& EventContext::operator++() {
        advance();
        return *this;
    }

    EventContext EventContext::operator++(int) {
        EventContext temp = *this;
        advance();
        return temp;
    }

    const clap_event_header* EventContext::next() noexcept {
       return m_nextEvent;
    }

    void EventContext::advance() {
        ++m_nextEventIndex;
        if (m_nextEventIndex >= m_inputEventQueueSize) {
            m_nextEvent = nullptr;
        } else {
            m_nextEvent = m_inputEventQueue->get(m_inputEventQueue, m_nextEventIndex);
        }

    }
}