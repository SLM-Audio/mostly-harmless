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
    class TaskThread {
    public:
        ~TaskThread() noexcept;
        void perform();
        void stop(bool join) noexcept;
        void sleep();
        void wake();
        [[nodiscard]] bool isThreadRunning() const noexcept;
        std::function<void(void)> action{ nullptr };

    private:
        std::mutex m_mutex;
        std::atomic<bool> m_isThreadRunning{ false };
        std::atomic<bool> m_canWakeUp{ false };
        std::condition_variable m_conditionVariable;
        std::unique_ptr<std::thread> m_thread{ nullptr };
    };
} // namespace mostly_harmless::utils
#endif // MOSTLYHARMLESS_MOSTLYHARMLESS_TASKTHREAD_H
