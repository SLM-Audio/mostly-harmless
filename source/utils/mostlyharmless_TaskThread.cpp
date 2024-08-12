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

    void TaskThread::signalThreadShouldExit() {
        m_threadShouldExit.store(true);
    }

    bool TaskThread::threadShouldExit() const noexcept {
        return m_threadShouldExit;
    }
}