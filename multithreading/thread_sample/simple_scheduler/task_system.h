#pragma once

#include<vector>
#include <deque>
#include<mutex>
#include<condition_variable>
#include<thread>
#include<atomic>

using lock_t = std::unique_lock < std::mutex >;

class notification_queue_t
{
   std::deque<std::function<void()>> _q;
   
   bool _done{ false };
   std::mutex _mutex;
   std::condition_variable _ready;
   
public:
   
   /*
    * Notify that I have done the job assigned
    **/
   void done()
   {
      {
         lock_t lock{ _mutex };
         _done = true;
      }
      _ready.notify_all();
   }
   
   /*
    * Pop new job from a queue
    **/
   bool pop(std::function<void()>& x) {
      lock_t lock{ _mutex };
      
      while (_q.empty() && !_done)
         _ready.wait(lock);
      
      if (_q.empty())
         return false;
      
      x = std::move(_q.front());
      _q.pop_front();
      return true;
   }
   
   /*
    * Push new function into the queue
    **/
   template<typename F>
   void push(F&& f) {
      
      {
         lock_t lock{ _mutex };
         _q.emplace_back(std::forward<F>(f));
      }
      _ready.notify_one();
   }
};


/*
 *
 **/
class task_system_t
{
   
   const unsigned _count{ std::thread::hardware_concurrency() };
   std::vector<std::thread> _threads;
   std::vector<notification_queue_t> _q{ _count };
   std::atomic<int> _index{ 0 };
   
   void run(unsigned i)
   {
      while (true)
      {
         std::function<void()> f;
         if (!_q[i].pop(f))
            break;
         
         //execute my job
         f();
      }
   }
   
public:
   
   task_system_t()
   {
      for (unsigned n = 0; n != _count; ++n) {
         _threads.emplace_back([&, n]{ run(n); });
      }
   }
   
   ~task_system_t()
   {
      //complete all tasks
      for (auto& q : _q)
         q.done();
      
      //join all threads spawned
      for (auto& e : _threads)
         e.join();
   }
   
   template<typename F>
   void async(F&& f)
   {
      auto i = _index++;
      _q[i%_count].push(std::forward<F>(f));
   }
};