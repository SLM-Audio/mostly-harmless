//
// Created by Syl on 12/08/2024.
//
#include <catch2/catch_test_macros.hpp>
#include <mostly_harmless/utils/mostlyharmless_Timer.h>
#include <mostly_harmless/utils/mostlyharmless_Proxy.h>
#include <iostream>
#include <cmath>
#include <thread>

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
            while (callCount < 5)
                ;
            timer.stop(true);
            REQUIRE(callCount >= 5);
        }

        SECTION("Test out-of-scope timer") {
            std::atomic<int> count{ 0 };
            {
                mostly_harmless::utils::Timer scopedTimer;
                auto task = [&count]() -> void {
                    ++count;
                };
                scopedTimer.action = std::move(task);
                scopedTimer.run(1);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            REQUIRE(count == 0);
        }
        SECTION("Test proxy") {
            std::atomic<bool> wasInvalid{ false };
            mostly_harmless::utils::Timer proxyTimer;
            {
                int x{ 0 };
                auto proxy = mostly_harmless::utils::Proxy<int>::create(&x);
                auto timerCallback = [&wasInvalid, proxy]() -> void {
                    if (!proxy->isValid()) {
                        wasInvalid = true;
                        return;
                    }
                    auto& x = *proxy->getWrapped();
                    ++x;
                    std::cout << x << "\n";
                };
                proxyTimer.action = std::move(timerCallback);
                proxyTimer.run(1);
                proxy->null();
            }
            std::this_thread::sleep_for(std::chrono::milliseconds{ 40 });
            REQUIRE(wasInvalid);
        }
    }
} // namespace mostly_harmless::tests