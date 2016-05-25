#pragma once

#include <mutex>
#include <condition_variable>
#include <memory>

namespace message_queue_system
{
   template<typename T>
   class message_queue_fine_grain_locking
   {
   private:

      struct node_t 
      {
         std::shared_ptr<T> _data;
         std::unique_ptr<node_t> _next;
      };

      std::unique_ptr<node_t> _head;
      node_t* _tail;
      std::mutex _head_mutex;
      std::mutex _tail_mutex;
      std::condition_variable _cv;
      std::atomic<bool> _done{ false };

      using lock_guard = std::lock_guard < std::mutex >;
      using lock_t = std::unique_lock < std::mutex >;

   private:

      node_t* get_tail()
      {
         lock_guard lk{ _tail_mutex };
         return _tail;
      }

   public:

      using value_type = T;

      //ctor
      message_queue_fine_grain_locking();
      message_queue_fine_grain_locking(const message_queue_fine_grain_locking&) = delete;
      message_queue_fine_grain_locking& operator=(const message_queue_fine_grain_locking&) = delete;

      //interface for termination
      bool is_terminated();
      void done();

      //interface to push/pop new task in the queue
      bool try_pop(value_type& x);
      bool pop(value_type& x);
      bool try_push(value_type f);
      void push(value_type f);

      //misc interface
      bool is_empty();
      void resize(const std::size_t&);
   };

   ///
   /// Implementation
   ///

   ///
   /// @brief: ctor of the message queue
   ///
   template<typename T>
   message_queue_fine_grain_locking<T>::message_queue_fine_grain_locking() :
      _head(std::make_unique<node_t>()), _tail(_head.get())
   {}

   ///
   /// @brief: check if the queue has been terminated
   ///
   template<typename T>
   bool message_queue_fine_grain_locking<T>::is_terminated()
   {
      return _done.load();
   }

   ///
   /// @brief: set the done flag in order to terminate  the queue
   ///
   template<typename T>
   void message_queue_fine_grain_locking<T>::done()
   {
      _done.store(true);
      _cv.notify_all();
   }

   ///
   /// @brief: pop a new message out from the queue
   ///
   template<typename T>
   bool message_queue_fine_grain_locking<T>::pop(T& val)
   {
      //lock and wait for new items
      std::unique_lock<std::mutex> head_lock(_head_mutex);

      _cv.wait(head_lock, [&]{
         return (_done) || (_head.get() != get_tail());
      });

      if (_done)
         return false;

      //extract data
      val = std::move(*_head->_data);
      //delete the head
      std::unique_ptr<node_t> old_head = std::move(_head);
      _head = std::move(old_head->_next);

      return true;
   }

   ///
   /// @brief: try to pop a new message out from the queue
   /// @return: true if operation succeeded, false otherwise
   ///
   template<typename T>
   bool message_queue_fine_grain_locking<T>::try_pop(T& val)
   {

      //try to lock and wait for new items
      lock_t head_lock(_head_mutex, std::try_to_lock);

      if (!head_lock)
         return false;

      _cv.wait(head_lock, [&] {
         return (_done) || (_head.get() != get_tail());
      });

      if (_done)
         return false;

      //extract data
      val = std::move(*_head->_data);
      //delete the head
      std::unique_ptr<node_t> old_head = std::move(_head);
      _head = std::move(old_head->_next);

      return true;
   }

   ///
   /// @brief: pushes a new element in the queue
   ///
   template<typename T>
   void message_queue_fine_grain_locking<T>::push(T val)
   {
      //copy data and build a new node
      auto new_data = std::make_shared<T>(std::move(val));
      std::unique_ptr<node_t> p = std::make_unique<node_t>();
      {
         lock_guard lk{_tail_mutex};
         //connect new node to the list
         _tail->_data = new_data;
         node_t* const new_tail = p.get();
         _tail->_next = std::move(p);
         _tail = new_tail;
      }
      _cv.notify_one();
   }

   ///
   /// @brief: attemps to push a new element in the queue
   /// @return: true/false if the element has been pushed successfuly
   ///
   template<typename T>
   bool message_queue_fine_grain_locking<T>::try_push(T val)
   {
      //copy data and build a new node
      auto new_data = std::make_shared<T>(std::move(val));
      std::unique_ptr<node_t> p = std::make_unique<node_t>();
      {
         //lock tail
         lock_t tail_lock{ _tail_mutex, std::try_to_lock };

         if (!tail_lock)
            return false;
         
         //connect new node to the list
         _tail->_data = new_data;
         node_t* const new_tail = p.get();
         _tail->_next = std::move(p);
         _tail = new_tail;
      }
      _cv.notify_one();
      return true;
   }

   ///
   /// @brief: check if the queue is empty
   ///
   template<typename T>
   bool message_queue_fine_grain_locking<T>::is_empty()
   {
      lock_guard heak_lock{ _head_mutex };
      return (_head.get() == get_tail());
   }

   template<typename T>
   void message_queue_fine_grain_locking<T>::resize(const std::size_t& n)
   {
   }

}
