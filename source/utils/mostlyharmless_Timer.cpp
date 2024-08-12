//
// Created by Syl on 12/08/2024.
//
#include <mostly_harmless/utils/mostlyharmless_Timer.h>
namespace mostly_harmless::utils {
   Timer::~Timer() noexcept {
       stop();
   }

   void Timer::run(int intervalMs) {
       if(!action) return;
       auto threadAction = [this, intervalMs]() -> void {
           auto startPoint = std::chrono::steady_clock::now();
           while(!m_thread.threadShouldExit()) {
               std::this_thread::sleep_for(std::chrono::milliseconds(1));
               if(m_thread.threadShouldExit()) return;
               const auto now = std::chrono::steady_clock::now();
               const auto delta = std::chrono::duration_cast<std::chrono::milliseconds>( now - startPoint);
               if(delta < std::chrono::milliseconds(intervalMs)) continue;
               action();
               startPoint = std::chrono::steady_clock::now();
           }
       };
       m_thread.action = std::move(threadAction);
       m_thread.perform();
   }

   void Timer::run(double frequency) {
       const auto seconds = 1.0 / frequency;
       const auto ms = static_cast<int>(seconds * 1000.0);
       run(ms);
   }

   void Timer::stop() {
       if(m_thread.isThreadRunning()) {
           m_thread.signalThreadShouldExit();
       }
   }

   bool Timer::isTimerRunning() const noexcept {
       return m_thread.isThreadRunning();
   }
}