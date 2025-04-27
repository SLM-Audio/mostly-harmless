//
// Created by Syl on 12/08/2024.
//
#include <mostly_harmless/utils/mostlyharmless_Timer.h>
#include <thread>
namespace mostly_harmless::utils {
    void Timer::run(int intervalMs) {
        if (!action || m_thread.isThreadRunning()) return;
        auto threadAction = [this, intervalMs]() -> void {
            while (m_thread.isThreadRunning()) {
                std::unique_lock<std::mutex> sl{ m_mutex };
                m_condVar.wait_for(sl, std::chrono::milliseconds(intervalMs), [this]() -> bool {
                    return !m_thread.isThreadRunning();
                });
                action();
            }
        };
        m_thread.action = std::move(threadAction);
        m_thread.perform();
    }

    void Timer::run(double frequency) {
        const auto intervalMs = static_cast<int>(1.0 / frequency);
        run(intervalMs);
    }

    void Timer::stop(bool join) {
        m_thread.stop(join);
    }


} // namespace mostly_harmless::utils