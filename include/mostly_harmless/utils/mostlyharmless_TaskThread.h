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
        auto perform() -> void;
        auto stop() noexcept -> void;
        auto sleep() -> void;
        auto wake() -> void;
        [[nodiscard]] auto isThreadRunning() const noexcept -> bool;
        std::function<void(void)> action{ nullptr };

    private:
        struct {
            std::mutex mutex;
            std::atomic<bool> canWakeUp{ false };
            std::condition_variable conditionVariable;
        } m_sleepState;
        std::atomic<bool> m_isThreadRunning{ false };
        std::unique_ptr<std::thread> m_thread{ nullptr };
    };
} // namespace mostly_harmless::utils
#endif // MOSTLYHARMLESS_MOSTLYHARMLESS_TASKTHREAD_H
