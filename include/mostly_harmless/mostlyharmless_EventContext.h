#ifndef MOSTLYHARMLESS_EVENTCONTEXT_H
#define MOSTLYHARMLESS_EVENTCONTEXT_H
#include "clap/events.h"
namespace mostly_harmless {
    struct EventContext {
        explicit EventContext(const clap_input_events_t* inputEventQueue) : m_inputEventQueue(inputEventQueue) {
            m_inputEventQueueSize = m_inputEventQueue->size(m_inputEventQueue);
            if (m_inputEventQueueSize == 0) return;
            m_nextEvent = m_inputEventQueue->get(m_inputEventQueue, m_nextEventIndex);
        }

        EventContext& operator++() {
            advance();
            return *this;
        }

        EventContext operator++(int) {
            EventContext temp = *this;
            advance();
            return temp;
        }

        [[nodiscard]] const clap_event_header_t* next() noexcept {
            return m_nextEvent;
        }

    private:
        void advance() {
            ++m_nextEventIndex;
            if (m_nextEventIndex >= m_inputEventQueueSize) {
                m_nextEvent = nullptr;
            } else {
                m_nextEvent = m_inputEventQueue->get(m_inputEventQueue, m_nextEventIndex);
            }
        }
        const clap_input_events_t* m_inputEventQueue{ nullptr };
        std::uint32_t m_inputEventQueueSize;
        const clap_event_header_t* m_nextEvent{ nullptr };
        std::uint32_t m_nextEventIndex{ 0 };
    };
} // namespace mostly_harmless
#endif