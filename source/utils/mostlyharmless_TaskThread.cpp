//
// Created by Syl on 12/08/2024.
//
#include "mostly_harmless/utils/mostlyharmless_OnScopeExit.h"


#include <mostly_harmless/utils/mostlyharmless_TaskThread.h>
#include <cassert>
#include <thread>
namespace mostly_harmless::utils {
    TaskThread::~TaskThread() noexcept {
        stop();
    }

    auto TaskThread::perform() -> void {
        auto expected{ false };
        if (m_isThreadRunning.compare_exchange_strong(expected, true)) {
            auto actionWrapper = [this]() -> void {
                OnScopeExit se{ [this]() -> void {
                    m_isThreadRunning.store(false);
                } };
                action();
            };
            m_thread = std::make_unique<std::thread>(std::move(actionWrapper));
        }
    }

    auto TaskThread::stop() noexcept -> void {
        m_isThreadRunning = false;
        if (!m_thread) {
            return;
        }
        if (m_thread->joinable()) {
            m_thread->join();
        }
        m_thread.reset();
    }

    auto TaskThread::sleep() -> void {
        m_sleepState.canWakeUp = false;
        std::unique_lock<std::mutex> ul{ m_sleepState.mutex };
        m_sleepState.conditionVariable.wait(ul, [this]() -> bool { return m_sleepState.canWakeUp; });
    }

    auto TaskThread::wake() -> void {
        m_sleepState.canWakeUp = true;
        std::lock_guard<std::mutex> lock{ m_sleepState.mutex };
        m_sleepState.conditionVariable.notify_one();
    }

    auto TaskThread::isThreadRunning() const noexcept -> bool {
        return m_isThreadRunning;
    }

} // namespace mostly_harmless::utils