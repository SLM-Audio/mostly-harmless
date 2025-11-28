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
        auto run(int intervalMs) -> void;
        auto run(double frequency) -> void;
        auto stop() -> void;
        std::function<void(void)> action;

    private:
        TaskThread m_thread;
    };
} // namespace mostly_harmless::utils
#endif // MOSTLYHARMLESS_MOSTLYHARMLESS_TIMER_H
