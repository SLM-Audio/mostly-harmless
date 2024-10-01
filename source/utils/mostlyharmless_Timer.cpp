//
// Created by Syl on 12/08/2024.
//
#include <mostly_harmless/utils/mostlyharmless_Timer.h>
#include <thread>
namespace mostly_harmless::utils {
    class Timer::Impl : public std::enable_shared_from_this<Timer::Impl> {
    private:
        struct Private final {
            explicit Private() = default;
        };

    public:
        // constructor is only usable by this class
        explicit Impl(Private) {}

        static std::shared_ptr<Impl> create() noexcept {
            return std::make_shared<Impl>(Private{});
        }

        void run(int intervalMs) {
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

        void run(double frequency) {
            const auto seconds = 1.0 / frequency;
            const auto ms = static_cast<int>(seconds * 1000.0);
            run(ms);
        }

        void stop() {
            m_thread.signalThreadShouldExit();
        }

        [[nodiscard]] bool isTimerRunning() const noexcept {
            return m_thread.isThreadRunning();
        }

        std::function<void(void)> action{ nullptr };

    private:
        TaskThread m_thread;
    };

    Timer::Timer() : m_impl(Timer::Impl::create()) {
    }

    Timer::~Timer() noexcept {
        m_impl->stop();
    }

    void Timer::run(int intervalMs) {
        m_impl->run(intervalMs);
    }

    void Timer::run(double frequency) {
        m_impl->run(frequency);
    }

    void Timer::stop() {
        m_impl->stop();
    }

    bool Timer::isTimerRunning() const noexcept {
        return m_impl->isTimerRunning();
    }

    void Timer::setAction(std::function<void()>&& action) noexcept {
        m_impl->action = std::move(action);
    }

} // namespace mostly_harmless::utils