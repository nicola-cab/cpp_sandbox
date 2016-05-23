#pragma once

#ifdef WIN32
#include <windows.h>
#endif

#include <chrono>

using Time = long long;

namespace time_util
{

#ifdef WIN32
   
   ///
   /// @brief: high resolution clock for windows (vs2013 does not support C++11 definition of high resolution clock)
   ///
   struct high_resolution_clock_t
   {
      typedef long long rep;
      typedef std::nano period;
      typedef std::chrono::duration<rep, period> duration;
      typedef std::chrono::time_point<high_resolution_clock_t>  time_point;
      static const bool is_steady = true;

      static time_point now()
      {
         auto f = []()  {
            LARGE_INTEGER frequency;
            QueryPerformanceFrequency(&frequency);
            return frequency.QuadPart;
         };

         const long long g_Frequency = f();

         LARGE_INTEGER count;
         QueryPerformanceCounter(&count);
         return time_point(duration(count.QuadPart * static_cast<rep>(period::den) / g_Frequency));
      }
   };

#else

   /// @brief: simple high resolution clock

   struct high_resolution_clock_t
   {
      typedef long long rep;
      typedef std::nano period;
      typedef std::chrono::duration<rep, period> duration;
      typedef std::chrono::time_point<std::chrono::high_resolution_clock>  time_point;
      static const bool is_steady = true;

      static time_point now()
      {
         return std::chrono::high_resolution_clock::now();
      }
   };

#endif

   
   ///
   /// @brief: timer implementation
   ///

   template<typename ClockImpl>
   class Timer
   {
   private:
      //std::clock_t _clock;
      using Clock = ClockImpl;
      typename Clock::time_point _start;
      typename Clock::time_point _end;
   public:
      using time_point = typename Clock::time_point;
      using ns = std::chrono::nanoseconds;
      using us = std::chrono::microseconds;
      using ms = std::chrono::milliseconds;
      using s = std::chrono::seconds;
      using m = std::chrono::minutes;

      void start()
      {
         _start = Clock::now();
      }

      void end()
      {
         _end = Clock::now();
      }

      template<typename Frame>
      long long elapsed()
      {
         return std::chrono::duration_cast<Frame>(_end - _start).count();
      }

      template<typename Frame>
      long long instant_of_time()
      {
         return std::chrono::duration_cast<Frame>(_end.time_since_epoch()).count();

      }

   };

}

using Timer = time_util::Timer < time_util::high_resolution_clock_t >;