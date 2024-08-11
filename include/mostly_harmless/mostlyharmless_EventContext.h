#ifndef MOSTLYHARMLESS_EVENTCONTEXT_H
#define MOSTLYHARMLESS_EVENTCONTEXT_H
#include "clap/events.h"
namespace mostly_harmless {
    /**
     * \brief Trivially copyable wrapper around the clap-api provided `clap_input_events` queue, for easier iterating.
     *
     */
    struct EventContext {
        /**
         * \param inputEventQueue The clap provided clap_input_events queue.
         */
        explicit EventContext(const clap_input_events_t* inputEventQueue);
        /**
         * Prefix increment - Advances the internal iterator to the next event in the queue.
         */
        EventContext& operator++();
        /**
         * Postfix increment - Advances the internal iterator to the next event in the queue.
         */
        EventContext operator++(int);

        /**
         * Attempts to access the next event in the queue - note that this could be a nullptr.
         * \return The next event in the queue.
         */
        [[nodiscard]] const clap_event_header_t* next() noexcept;

    private:
        void advance();

        const clap_input_events_t* m_inputEventQueue{ nullptr };
        std::uint32_t m_inputEventQueueSize;
        const clap_event_header_t* m_nextEvent{ nullptr };
        std::uint32_t m_nextEventIndex{ 0 };
    };
} // namespace mostly_harmless
#endif