//
// Created by Syl on 12/08/2024.
//
#include <mostly_harmless/utils/mostlyharmless_Timer.h>
#include <thread>
namespace mostly_harmless::utils {
    Timer::~Timer() noexcept {
        stop();
    }

    void Timer::run(int intervalMs) {
        if (!action) return;
        auto weakThis = weak_from_this();
        auto threadAction = [weakThis, intervalMs]() -> void {
            auto startPoint = std::chrono::steady_clock::now();
            while (true) {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));

                auto self = weakThis.lock();
                if (!self || self->m_thread.threadShouldExit()) return;

                const auto now = std::chrono::steady_clock::now();
                const auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(now - startPoint);
                if (delta < std::chrono::milliseconds(intervalMs)) continue;

                self->action();
                startPoint = now;
            }
        };
        m_thread.action = std::move(threadAction);
        m_thread.perform();
    }

    void Timer::run(double frequency) {
        const auto seconds = 1.0 / frequency;
        const auto ms = static_cast<int>(seconds * 1000.0);
        run(ms);
    }

    void Timer::stop() {
        m_thread.signalThreadShouldExit();
    }

    bool Timer::isTimerRunning() const noexcept {
        return m_thread.isThreadRunning();
    }
} // namespace mostly_harmless::utils