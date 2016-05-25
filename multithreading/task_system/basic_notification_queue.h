#pragma once

#include<deque>
#include<mutex>
#include<condition_variable>

namespace message_queue_system
{
   using lock_t = std::unique_lock < std::mutex >;
   
   ///
   /// @brief: basic notification queue
   ///
   class notification_queue_t
   {
      std::deque<std::function<void()>> _q;
      bool _done{ false };
      std::mutex _mutex;
      std::condition_variable _ready;
      
   public:
      
      bool is_terminated();
      void done();
      
      bool try_pop(std::function<void()>& x);
      bool pop(std::function<void()>& x);
      
      template<typename F> bool try_push(F&& f);
      template<typename F> void push(F&& f);
      
   };
   
   ///
   /// Implementation
   ///
   
   
   ///
   /// @brief: check if the queue is terminated
   ///
   bool notification_queue_t::is_terminated()
   {
      lock_t lock{ _mutex };
      return _done;
   }
   
   ///
   /// @brief: set the done flag in order to terminate  the queue
   ///
   void notification_queue_t::done()
   {
      {
         lock_t lock{ _mutex };
         _done = true;
      }
      _ready.notify_all();
   }
   
   ///
   /// @brief: try to pop some function to execute out from queue
   ///
   bool notification_queue_t::try_pop(std::function<void()>& x)
   {
      lock_t lock{ _mutex, std::try_to_lock };
      
      if (!lock || _q.empty())
         return false;
      
      x = std::move(_q.front());
      _q.pop_front();
      return true;
   }
   
   ///
   /// @brief: pop from queue
   ///
   bool notification_queue_t::pop(std::function<void()>& x)
   {
      lock_t lock{ _mutex };
      
      while (_q.empty() && !_done)
         _ready.wait(lock);
      
      if (_q.empty())
         return false;
      
      x = std::move(_q.front());
      _q.pop_front();
      return true;
   }
   
   ///
   /// @brief: speculively try to push into the queue
   ///
   template<typename F>
   bool notification_queue_t::try_push(F&& f)
   {
      {
         lock_t lock{ _mutex, std::try_to_lock };
         
         if (!lock)
            return false;
         
         _q.emplace_back(std::forward<F>(f));
      }
      _ready.notify_one();
      return true;
   }
   
   ///
   /// @brief: push into queue
   ///
   template<typename F>
   void notification_queue_t::push(F&& f)
   {
      {
         lock_t lock{ _mutex };
         _q.emplace_back(std::forward<F>(f));
      }
      _ready.notify_one();
   }
   
}

