//
// Created by Syl on 12/08/2024.
//

#ifndef MOSTLYHARMLESS_MOSTLYHARMLESS_TIMER_H
#define MOSTLYHARMLESS_MOSTLYHARMLESS_TIMER_H
#include <mostly_harmless/utils/mostlyharmless_TaskThread.h>
#include <chrono>
namespace mostly_harmless::utils {
    /**
     * \brief Class to periodically perform a user defined action.
     *
     * Runs on its own thread - so thread safety concerns all apply here.
     */
    class Timer {
    public:
        /**
         * Attempts to stop the timer if its running - note that this won't be instanteous (see stop() )
         */
        ~Timer() noexcept;
        /**
         * Starts the timer if action is not null (returns otherwise) - the action will be performed every intervalMs milliseconds (the first call will be after intervalMs milliseconds - not immediate).
         * \param intervalMs The interval between calls, in milliseconds.
         */
        void run(int intervalMs);
        /**
         * Starts the timer if action is not null (returns otherwise) - the action will be performed at frequency hz (the first call is not immediate).
         * \param frequency The interval between calls, in hz.
         */
        void run(double frequency);
        /**
         * Signals the timer thread that it should stop. The thread checks the flag every millisecond, so account for a worst case delay of around that.
         */
        void stop();

        /**
         * \return true if the timer is running, false otherwise.
         */
        [[nodiscard]] bool isTimerRunning() const noexcept;
        /**
         * A user defined callback to be performed periodically by the timer thread. Again, remember that this is *not* happening on the caller thread, so be extremely careful to write thread safe code here.
         */
        std::function<void(void)> action{ nullptr };
    private:
        TaskThread m_thread;
    };
}
#endif // MOSTLYHARMLESS_MOSTLYHARMLESS_TIMER_H
