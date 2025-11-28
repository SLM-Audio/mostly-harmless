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
            std::mutex mutex;
            std::condition_variable cv;
            std::atomic<int> callCount{ 0 };
            auto start = std::chrono::steady_clock::now();
            auto timerCallback = [&callCount, &start, &mutex, &cv]() -> void {
                const auto now = std::chrono::steady_clock::now();
                const auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(now - start);
                // normalise, and truncate..
                const auto normalisedDelta = static_cast<double>(delta.count()) / 100.0;
                const auto truncatedDelta = std::round(normalisedDelta);
                REQUIRE(truncatedDelta == 1);
                start = std::chrono::steady_clock::now();
                {
                    std::lock_guard<std::mutex> lock{ mutex };
                    ++callCount;
                }
                cv.notify_one();
            };
            timer.action = std::move(timerCallback);
            timer.run(static_cast<int>(100));
            std::unique_lock<std::mutex> ul{ mutex };
            cv.wait_for(ul, std::chrono::seconds{ 1 }, [&callCount]() -> bool {
                return callCount == 5;
            });
            REQUIRE(callCount == 5);
            timer.stop();
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
            REQUIRE(count == 0);
        }
        SECTION("Test proxy") {
            std::atomic<bool> wasInvalid{ false };
            std::mutex mutex;
            std::condition_variable cv;
            mostly_harmless::utils::Timer proxyTimer;
            {
                int x{ 0 };
                auto proxy = mostly_harmless::utils::Proxy<int>::create(&x);
                auto timerCallback = [&wasInvalid, &mutex, &cv, proxy]() -> void {
                    if (!proxy->isValid()) {
                        {
                            std::lock_guard<std::mutex> lock{ mutex };
                            wasInvalid = true;
                        }
                        cv.notify_one();
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
            std::unique_lock<std::mutex> ul{ mutex };
            cv.wait_for(ul, std::chrono::seconds{ 1 }, [&]() -> bool {
                return wasInvalid;
            });
            REQUIRE(wasInvalid);
        }
    }
} // namespace mostly_harmless::tests