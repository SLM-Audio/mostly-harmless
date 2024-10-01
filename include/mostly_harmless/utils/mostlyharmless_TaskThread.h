//
// Created by Syl on 12/08/2024.
//

#ifndef MOSTLYHARMLESS_MOSTLYHARMLESS_TASKTHREAD_H
#define MOSTLYHARMLESS_MOSTLYHARMLESS_TASKTHREAD_H
#include <functional>
#include <atomic>
#include <mutex>
namespace mostly_harmless::utils {
    /**
     * \brief Wrapper around a std::thread to perform a single action, then exit.
     *
     * Note that `perform()` will detach the thread, so it's no longer joinable - so it's your responsibility to ensure the thread elegantly exits of its own volition.
     * When the action is performed, a wrapper lambda automatically informs and internal atomic that the thread is running, and when the action has completed, it sets it back to false.
     */
    class TaskThread {
    public:
        /**
         * \private
         */
        TaskThread();
        /**
         * Starts the thread, with the user provided `action` lambda if it has been assigned - if it hasn't, asserts false and exits.
         * Note that launching a thread is a syscall, and thus can take an unbounded amount of time - so this is *not* realtime safe,
         * and you can't really make any assumptions about how long until isThreadRunning() will return true.
         */
        void perform();

        /**
         * Sleeps the thread until wake() is called. <b>Only call this from the thread!</b>
         */
        void sleep();

        /**
         * Wakes a thread previously suspended with `sleep()`.
         */
        void wake();

        /**
         * Sets an internal atomic bool to specify that the thread should exit, accessible through `threadShouldExit`. Note that this doesn't actually kill the thread,
         * it's up to you to do something with the bool.
         */
        void signalThreadShouldExit();

        /**
         * \return The state of the internal atomic bool specifying whether or not the thread should exit.
         */
        [[nodiscard]] bool threadShouldExit() const noexcept;

        /**
         * \return Whether or not the thread is currently running.
         */
        [[nodiscard]] bool isThreadRunning() const noexcept;

        /**
         * The action that should actually be performed on the thread.
         */
        void setAction(std::function<void(void)>&& action) noexcept;

    private:
        class Impl;
        std::shared_ptr<Impl> m_impl;
    };
} // namespace mostly_harmless::utils
#endif // MOSTLYHARMLESS_MOSTLYHARMLESS_TASKTHREAD_H
