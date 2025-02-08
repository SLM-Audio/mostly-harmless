//
// Created by Syl on 12/08/2024.
//
#include <mostly_harmless/utils/mostlyharmless_Timer.h>
#include <thread>
namespace mostly_harmless::utils {
    void Timer::run(int intervalMs) {
        if (!action || m_thread.isThreadRunning()) return;
        auto threadAction = [this, intervalMs]() -> void {
            auto startPoint = std::chrono::steady_clock::now();
            while (m_thread.isThreadRunning()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
                const auto now = std::chrono::steady_clock::now();
                const auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(now - startPoint);
                if (delta < std::chrono::milliseconds(intervalMs)) continue;
                action();
                startPoint = std::chrono::steady_clock::now();
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