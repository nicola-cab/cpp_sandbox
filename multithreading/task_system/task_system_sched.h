#pragma once

#include<vector>
#include<deque>
#include<thread>

namespace task_system
{
   ///
   /// @brief: 1 queue per thread task scheduling system
   ///
   template< typename Queue>
   class task_system_queue_thread_t
   {
      const unsigned _count{ std::thread::hardware_concurrency() };
      std::vector<std::thread> _threads;
      
#ifdef _WIN32
      std::deque<Queue> _q;
#else
      std::vector<Queue> _q{ _count };
#endif
      
      std::atomic<int> _index{ 0 };
      
      void run(unsigned i)
      {
         while (true)
         {
            std::function<void()> f;
            if (!_q[i].pop(f))
               break;
            f();
         }
      }
      
   public:
      
      task_system_queue_thread_t()
      {
#ifdef _WIN32
         _q.resize(_count);
#endif
         for (unsigned n = 0; n != _count; ++n) {
            _threads.emplace_back([&, n]{ run(n); });
         }
      }
      
      ~task_system_queue_thread_t()
      {
         for (auto& q : _q) q.done();
         for (auto& e : _threads) e.join();
      }
      
      template<typename F>
      void async(F&& f)
      {
         auto i = _index++;
         _q[i%_count].push(std::forward<F>(f));
      }
   };
}

