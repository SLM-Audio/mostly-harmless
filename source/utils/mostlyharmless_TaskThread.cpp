//
// Created by Syl on 12/08/2024.
//
#include <mostly_harmless/utils/mostlyharmless_TaskThread.h>
#include <cassert>
#include <thread>
namespace mostly_harmless::utils {
    void TaskThread::perform() {
        if(!action) {
            assert(false);
            return;
        }
        auto actionWrapper = [this]() -> void {
            m_isThreadRunning.store(true);
            action();
            m_isThreadRunning.store(false);
        };

        std::thread worker{ std::move(actionWrapper) };
        worker.detach();
    }

    void TaskThread::sleep() {
        m_canWakeUp = false;
        std::unique_lock<std::mutex> lock(m_mutex);
        m_conditionVariable.wait(lock, [this]() -> bool{ return m_canWakeUp; });
    }

    void TaskThread::wake() {
        std::lock_guard<std::mutex> guard{ m_mutex };
        m_canWakeUp = true;
        m_conditionVariable.notify_one();
    }

    void TaskThread::signalThreadShouldExit() {
        m_threadShouldExit.store(true);
    }

    bool TaskThread::threadShouldExit() const noexcept {
        return m_threadShouldExit;
    }

    bool TaskThread::isThreadRunning() const noexcept {
        return m_isThreadRunning;
    }
}