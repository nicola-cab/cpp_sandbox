#pragma once

#include<vector>
#include<deque>
#include<thread>

namespace task_system
{
   ///
   /// @brief: simplest task system with one queue and multiple consumers
   ///         competing each other to acquire the lock
   ///
   template< typename Queue>
   class task_system_t
   {
      const unsigned _count{ std::thread::hardware_concurrency() };
      std::vector<std::thread> _threads;
      Queue _q;
      
      void run(unsigned i)
      {
         while (true)
         {
            std::function<void()> f;
            if (!_q.pop(f))
               break;
            f();
         }
      }
      
   public:
      
      task_system_t()
      {
         for (unsigned n = 0; n != _count; ++n)
         {
            _threads.emplace_back([&, n]{ run(n); });
         }
      }
      
      ~task_system_t()
      {
         _q.done();
         for (auto& e : _threads)
            e.join();
      }
      
      template<typename F>
      void async(F&& f)
      {
         _q.push(std::forward<F>(f));
      }
   };
}