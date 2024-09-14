//
// Created by Syl on 12/08/2024.
//
#include <catch2/catch_test_macros.hpp>
#include <mostly_harmless/utils/mostlyharmless_Timer.h>
#include <iostream>
#include <cmath>

namespace mostly_harmless::tests {
    TEST_CASE("Test Timer") {
        mostly_harmless::utils::Timer timer;
        SECTION("Test calls") {
            std::atomic<int> callCount{ 0 };
            auto start = std::chrono::steady_clock::now();
            auto timerCallback = [&callCount, &start]() -> void {
                const auto now = std::chrono::steady_clock::now();
                const auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(now - start);
                // normalise, and truncate..
                const auto normalisedDelta = static_cast<double>(delta.count()) / 100.0;
                const auto truncatedDelta = std::round(normalisedDelta);
                REQUIRE(truncatedDelta == 1);
                start = std::chrono::steady_clock::now();
                ++callCount;
            };
            timer.action = std::move(timerCallback);
            timer.run(static_cast<int>(100));
            while(callCount < 5);
            timer.stop();
            REQUIRE(callCount >= 5);
        }
    }
}