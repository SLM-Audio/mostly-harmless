//
// Created by Syl on 12/08/2024.
//
#include <catch2/catch_test_macros.hpp>
#include <mostly_harmless/utils/mostlyharmless_TaskThread.h>
#include <mutex>
#include <thread>
namespace mostly_harmless::testing {
    TEST_CASE("Test TaskThread") {
        std::mutex mutex;
        mostly_harmless::utils::TaskThread taskThread;
        SECTION("Wait for lock") {
            auto x{ false };
            auto task = [&mutex, &x]() -> void {
                std::scoped_lock<std::mutex> sl{ mutex };
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                x = true;
            };
            taskThread.action = std::move(task);
            taskThread.perform();
            // give it a bit of time to spin up.. (remember that this is a syscall)
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
            REQUIRE(taskThread.isThreadRunning());
            // Sleep for a ms so the task has a chance to acquire the mutex..
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            std::scoped_lock<std::mutex> sl{ mutex };
            REQUIRE(x);
            REQUIRE(!taskThread.isThreadRunning());
        }

        SECTION("Kill") {
            auto task = [&taskThread]() -> void {
                while(!taskThread.threadShouldExit());
            };
            taskThread.action = std::move(task);
            taskThread.perform();
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            REQUIRE(taskThread.isThreadRunning());
            taskThread.signalThreadShouldExit();
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            REQUIRE(!taskThread.isThreadRunning());
        }

        SECTION("Sleep/Wake") {
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
}
