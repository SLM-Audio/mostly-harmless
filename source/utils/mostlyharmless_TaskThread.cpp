//
// Created by Syl on 12/08/2024.
//
#include <mostly_harmless/utils/mostlyharmless_TaskThread.h>
#include <cassert>
#include <thread>
namespace mostly_harmless::utils {
    TaskThread::~TaskThread() noexcept {
        while (isThreadRunning()) {
            signalThreadShouldExit();
            wake();
        }
    }

    void TaskThread::perform() {
        if (m_isThreadRunning) return;
        m_threadShouldExit.store(false);
        m_threadAboutToStart = true;
        if (!action) {
            assert(false);
            return;
        }
        auto actionWrapper = [this]() -> void {
            m_threadAboutToStart.store(false);
            m_isThreadRunning.store(true);
            action();
            m_isThreadRunning.store(false);
        };
        m_threadShouldExit = false;
        std::thread worker{ std::move(actionWrapper) };
        worker.detach();
    }

    void TaskThread::sleep() {
        m_canWakeUp = false;
        std::unique_lock<std::mutex> lock(m_mutex);
        m_conditionVariable.wait(lock, [this]() -> bool { return m_canWakeUp; });
    }

    void TaskThread::wake() {
        m_canWakeUp = true;
        std::lock_guard<std::mutex> guard{ m_mutex };
        m_conditionVariable.notify_one();
    }

    void TaskThread::signalThreadShouldExit() {
        m_threadShouldExit.store(true);
    }

    bool TaskThread::threadShouldExit() const noexcept {
        return m_threadShouldExit;
    }

    bool TaskThread::isThreadRunning() const noexcept {
        return m_isThreadRunning || m_threadAboutToStart;
    }
} // namespace mostly_harmless::utils