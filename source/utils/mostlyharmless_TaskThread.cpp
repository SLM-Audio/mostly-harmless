//
// Created by Syl on 12/08/2024.
//
#include <mostly_harmless/utils/mostlyharmless_TaskThread.h>
#include <cassert>
#include <thread>
namespace mostly_harmless::utils {
    class TaskThread::Impl final : std::enable_shared_from_this<Impl> {
    public:
        static std::shared_ptr<Impl> create() noexcept {
            return std::make_shared<Impl>();
        }

        void perform() {
            if (m_isThreadRunning) return;
            if (!action) {
                assert(false);
                return;
            }
            auto weakThis = weak_from_this();
            auto actionWrapper = [weakThis]() -> void {
                if (auto self = weakThis.lock()) {
                    self->m_isThreadRunning.store(true);
                    self->action();
                    self->m_isThreadRunning.store(false);
                }
            };
            m_threadShouldExit = false;
            std::thread worker{ std::move(actionWrapper) };
            worker.detach();
        }

        void sleep() {
            m_canWakeUp = false;
            std::unique_lock<std::mutex> lock(m_mutex);
            m_conditionVariable.wait(lock, [this]() -> bool { return m_canWakeUp; });
        }

        void wake() {
            std::lock_guard<std::mutex> guard{ m_mutex };
            m_canWakeUp = true;
            m_conditionVariable.notify_one();
        }

        void signalThreadShouldExit() {
            m_threadShouldExit.store(true);
        }

        [[nodiscard]] bool threadShouldExit() const noexcept {
            return m_threadShouldExit;
        }

        [[nodiscard]] bool isThreadRunning() const noexcept {
            return m_isThreadRunning;
        }


        std::function<void(void)> action{ nullptr };

    private:
        std::mutex m_mutex;
        std::condition_variable m_conditionVariable;
        std::atomic<bool> m_canWakeUp{ false };
        std::atomic<bool> m_isThreadRunning{ false };
        std::atomic<bool> m_threadShouldExit{ false };
    };

    TaskThread::TaskThread() : m_impl(Impl::create()) {
    }

    void TaskThread::perform() {
        m_impl->perform();
    }

    void TaskThread::sleep() {
        m_impl->sleep();
    }

    void TaskThread::wake() {
        m_impl->wake();
    }

    void TaskThread::signalThreadShouldExit() {
        m_impl->signalThreadShouldExit();
    }

    bool TaskThread::threadShouldExit() const noexcept {
        return m_impl->threadShouldExit();
    }

    bool TaskThread::isThreadRunning() const noexcept {
        return m_impl->isThreadRunning();
    }

    void TaskThread::setAction(std::function<void()>&& action) noexcept {
        m_impl->action = std::move(action);
    }
} // namespace mostly_harmless::utils