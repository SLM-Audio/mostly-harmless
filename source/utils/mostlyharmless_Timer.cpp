//
// Created by Syl on 12/08/2024.
//
#include <mostly_harmless/utils/mostlyharmless_Timer.h>
#include <thread>
namespace mostly_harmless::utils {
    auto Timer::run(int intervalMs) -> void {
        if (!action || m_thread.isThreadRunning()) return;
        auto threadAction = [this, intervalMs]() -> void {
            auto startPoint = std::chrono::steady_clock::now();
            while (!m_thread.hasSignalledStop()) {
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

    auto Timer::run(double frequency) -> void {
        const auto intervalMs = static_cast<int>(1.0 / frequency);
        run(intervalMs);
    }

    auto Timer::stop() -> void {
        m_thread.stop();
    }


} // namespace mostly_harmless::utils