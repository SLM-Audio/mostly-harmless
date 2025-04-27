//
// Created by Syl on 12/08/2024.
//

#ifndef MOSTLYHARMLESS_MOSTLYHARMLESS_TIMER_H
#define MOSTLYHARMLESS_MOSTLYHARMLESS_TIMER_H
#include <mostly_harmless/utils/mostlyharmless_TaskThread.h>
#include <chrono>
namespace mostly_harmless::utils {
    class Timer final {
    public:
        void run(int intervalMs);
        void run(double frequency);
        void stop(bool join);
        std::function<void(void)> action;

    private:
        std::mutex m_mutex;
        std::condition_variable m_condVar;
        TaskThread m_thread;
    };
} // namespace mostly_harmless::utils
#endif // MOSTLYHARMLESS_MOSTLYHARMLESS_TIMER_H
