//
// Created by Syl on 12/08/2024.
//

#ifndef MOSTLYHARMLESS_MOSTLYHARMLESS_TASKTHREAD_H
#define MOSTLYHARMLESS_MOSTLYHARMLESS_TASKTHREAD_H
#include <functional>
#include <atomic>
#include <mutex>
#include <thread>

namespace mostly_harmless::utils {
    /***
     * \brief Convenience wrapper around std::thread
     *
     * Contains some simple signalling mechanisms, and forms the basis for `Timer` - to use it, supply a lambda to the `action` field, then call perform (and remember to call `stop` to join the thread!)
     */
    class TaskThread {
    public:
        /**
         * Destructor. Calls stop, which joins the internal thread, so will block if the thread is running!
         */
        ~TaskThread() noexcept;

        /**
         * Performs the `action` lambda (if it's been set) on a std::thread, and sets isThreadRunning to false on scope exit.
         */
        auto perform() -> void;

        /**
         * Calls signalStop(), joins the thread, and resets the internal state for reuse.
         */
        auto stop() noexcept -> void;

        /**
         * For use in your `action` lambda, pauses the thread's execution until it's woken via wake().
         * Internally just uses a `std::condition_variable` and an atomic "canWakeUp" bool - this call is equivalent to std::condition_variable::wait().
         * Note that this doesn't actually do anything on its own unless you call `sleep` in your action - it's just there if you need it!
         */
        auto sleep() -> void;

        /**
         * Paired with sleep, sets the `canWakeUp` bool to true, and notifies the underlying condition variable. All ramblings from wake()` also apply here.
         */
        auto wake() -> void;

        /**
         * Sets an internal atomic bool that the thread should exit. Like sleep() and wake(), doesn't actually do anything on its own,
         * pair this with usage of hasSignalledStop() in your custom thread action.
         */
        auto signalStop() -> void;

        /**
         * Retrieves the state of the atomic bool described in signalStop() - use this in your custom thread action to respond to cancellation requests if needed (again, does nothing on it's own!)
         * @return The state of the `should_stop` bool.
         */
        [[nodiscard]] auto hasSignalledStop() const noexcept -> bool;

        /**
         * Gives a loose indication of whether the thread is still running or not - loose, because this will only get set *after* the user action has been run, not within.
         * This means there may be a few cpu cycles discrepancy between when you *think* this should return true and when it *actually* returns true.
         * TLDR, don't rely on it for anything timing critical!
         * @return Whether the thread is running or not.
         */
        [[nodiscard]] auto isThreadRunning() const noexcept -> bool;

        /**
         *  A user-settable lambda to be invoked on the internal std::thread. Put your threaded logic here!!
         */
        std::function<void(void)> action{ nullptr };

    private:
        auto reset() -> void;
        struct {
            std::mutex mutex;
            std::atomic<bool> canWakeUp{ false };
            std::condition_variable conditionVariable;
        } m_sleepState;
        std::atomic<bool> m_isThreadRunning{ false };
        std::atomic<bool> m_stop{ false };
        std::unique_ptr<std::thread> m_thread{ nullptr };
    };
} // namespace mostly_harmless::utils
#endif // MOSTLYHARMLESS_MOSTLYHARMLESS_TASKTHREAD_H
