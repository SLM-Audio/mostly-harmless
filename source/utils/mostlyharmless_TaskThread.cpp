//
// Created by Syl on 12/08/2024.
//
#include <mostly_harmless/utils/mostlyharmless_TaskThread.h>
#include <cassert>
#include <thread>
namespace mostly_harmless::utils {
    TaskThread::~TaskThread() noexcept {
        stop(true);
    }

    void TaskThread::perform() {
        auto expected{ false };
        if (m_isThreadRunning.compare_exchange_strong(expected, true)) {
            auto actionWrapper = [this]() -> void {
                action();
                m_isThreadRunning = false;
            };
            m_thread = std::make_unique<std::thread>(std::move(actionWrapper));
        }
    }

    void TaskThread::stop(bool join) noexcept {
        m_isThreadRunning = false;
        if (!m_thread) {
            return;
        }
        if (join) {
            if (m_thread->joinable()) {
                m_thread->join();
            }
            m_thread.reset();
        }
    }

    void TaskThread::sleep() {
        m_canWakeUp = false;
        std::unique_lock<std::mutex> ul{ m_mutex };
        m_conditionVariable.wait(ul, [this]() -> bool { return m_canWakeUp; });
    }

    void TaskThread::wake() {
        m_canWakeUp = true;
        std::lock_guard<std::mutex> lock{ m_mutex };
        m_conditionVariable.notify_one();
    }

    bool TaskThread::isThreadRunning() const noexcept {
        return m_isThreadRunning;
    }

} // namespace mostly_harmless::utils