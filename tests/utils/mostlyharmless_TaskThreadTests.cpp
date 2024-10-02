//
// Created by Syl on 12/08/2024.
//
#include <catch2/catch_test_macros.hpp>
#include <mostly_harmless/utils/mostlyharmless_TaskThread.h>
#include <mutex>
#include <thread>
#include <iostream>
namespace mostly_harmless::testing {
    constexpr static size_t s_nRepeats{ 50 };
    TEST_CASE("Test TaskThread") {
        SECTION("Wait for lock") {
            for(auto i = 0; i < s_nRepeats; ++i) {
                mostly_harmless::utils::TaskThread taskThread;
                std::mutex mutex;
                auto x{ false };
                auto task = [&mutex, &x]() -> void {
                    std::scoped_lock<std::mutex> sl{ mutex };
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                    x = true;
                };
                taskThread.action = std::move(task);
                taskThread.perform();
                // Sleep so the task has a chance to acquire the mutex.. (syscall and all that)
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                std::scoped_lock<std::mutex> sl{ mutex };
                REQUIRE(x);
                REQUIRE(!taskThread.isThreadRunning());
            }
        }

        SECTION("Kill") {
            for(auto i = 0; i <s_nRepeats; ++i) {
                mostly_harmless::utils::TaskThread taskThread;
                auto task = [&taskThread]() -> void {
                    while (!taskThread.threadShouldExit())
                        ;
                };
                taskThread.action = std::move(task);
                taskThread.perform();
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
                REQUIRE(taskThread.isThreadRunning());
                taskThread.signalThreadShouldExit();
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
                REQUIRE(!taskThread.isThreadRunning());
            }
        }

        SECTION("Sleep/Wake") {
            for(auto i = 0; i < s_nRepeats; ++i) {
                mostly_harmless::utils::TaskThread taskThread;
                auto task = [&taskThread]() -> void {
                    taskThread.sleep();
                };
                taskThread.action = std::move(task);
                taskThread.perform();
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                REQUIRE(taskThread.isThreadRunning());
                taskThread.wake();
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
                REQUIRE(!taskThread.isThreadRunning());
            }
        }

        SECTION("Out of scope") {
            for (auto i = 0; i < s_nRepeats; ++i) {
                std::chrono::time_point<std::chrono::steady_clock> start;
                {
                    utils::TaskThread scopedThread;
                    auto task = [&scopedThread]() -> void {
                        while (!scopedThread.threadShouldExit()) {
                            scopedThread.sleep();
                        }
                    };
                    scopedThread.action = std::move(task);
                    scopedThread.perform();
                    start = std::chrono::steady_clock::now();
                }
                const auto end = std::chrono::steady_clock::now();
                const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
                REQUIRE(duration < std::chrono::milliseconds(5));
            }
        }
    }
} // namespace mostly_harmless::testing
