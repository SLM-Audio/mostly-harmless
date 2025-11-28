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
            for (size_t i = 0; i < s_nRepeats; ++i) {
                mostly_harmless::utils::TaskThread taskThread;
                std::mutex mutex;
                auto x{ false };
                std::condition_variable cv;
                auto task = [&mutex, &x, &cv]() -> void {
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                    {
                        std::lock_guard<std::mutex> lock{ mutex };
                        x = true;
                    }
                    cv.notify_all();
                };
                taskThread.action = std::move(task);
                taskThread.perform();
                std::unique_lock<std::mutex> ul{ mutex };
                cv.wait_for(ul, std::chrono::seconds{ 1 }, [&x]() -> bool {
                    return x == true;
                });
                REQUIRE(x);
            }
        }

        SECTION("Kill") {
            for (size_t i = 0; i < s_nRepeats; ++i) {
                mostly_harmless::utils::TaskThread taskThread;
                std::mutex mutex;
                std::condition_variable cv;
                auto task = [&taskThread, &cv]() -> void {
                    while (taskThread.isThreadRunning());
                    cv.notify_all();
                };

                taskThread.action = std::move(task);
                taskThread.perform();
                REQUIRE(taskThread.isThreadRunning());
                taskThread.stop();
                std::unique_lock<std::mutex> ul{ mutex };
                cv.wait_for(ul, std::chrono::seconds{ 1 }, [&]() -> bool { return !taskThread.isThreadRunning(); });
                REQUIRE(!taskThread.isThreadRunning());
            }
        }

        SECTION("Sleep/Wake") {
            for (size_t i = 0; i < s_nRepeats; ++i) {
                mostly_harmless::utils::TaskThread taskThread;
                std::mutex mutex;
                std::condition_variable cv;
                auto task = [&taskThread, &cv]() -> void {
                    taskThread.sleep();
                    cv.notify_one();
                };
                taskThread.action = std::move(task);
                taskThread.perform();
                REQUIRE(taskThread.isThreadRunning());
                std::this_thread::sleep_for(std::chrono::milliseconds{ 10 });
                taskThread.wake();
                taskThread.stop();
                std::unique_lock<std::mutex> ul{ mutex };
                cv.wait_for(ul, std::chrono::seconds{ 1 }, [&]() -> bool {
                    return !taskThread.isThreadRunning();
                });
                REQUIRE(!taskThread.isThreadRunning());
            }
        }
    }
} // namespace mostly_harmless::testing
