#pragma once

#include<vector>
#include<thread>
#include<atomic>
#include <deque>
#include "basic_notification_queue.h"

namespace task_system
{
   ///
   /// @brief: task system with queue per task and work stealing among consumers
   ///

   class task_system_task_stealing_t
   {
      const unsigned _count{ std::thread::hardware_concurrency() };
      std::vector<std::thread> _threads;

#ifdef _WIN32
      //deque because reserve does not work on windows with not copy constructable objects
      std::deque<notification_queue_t> _q;
#else
      std::vector<notification_queue_t> _q{ _count };
#endif
      std::atomic<int> _index{ 0 };

      void run(unsigned i)
      {
         while (true)
         {
            std::function<void()> f;

            for (unsigned n = 0; n != _count; ++n)
            {
               if (!_q[(i + n) % _count].try_pop(f))
                  break;
            }

            if (!f && !_q[i].pop(f))
               break;

            //std::cout << "Task id = "<< i << " ... running \n";

            f();

         }
      }

   public:

      task_system_task_stealing_t()
      {
#if defined (_WIN32)
         _q.resize(_count);
#endif
         for (unsigned n = 0; n != _count; ++n) {
            _threads.emplace_back([&, n]{ run(n); });
         }
      }

      ~task_system_task_stealing_t()
      {
         for (auto& q : _q)
            q.done();

         for (auto& e : _threads)
            e.join();
      }

      template<typename F>
      void async(F&& f)
      {
         auto i = _index++;

         for (unsigned n = 0; n != _count; ++n)
            if (_q[(i + n) % _count].try_push(std::forward<F>(f)))
               return;

         _q[i%_count].push(std::forward<F>(f));
      }
   };

}

