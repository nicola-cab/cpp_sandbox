#pragma once

#include <mutex>
#include <condition_variable>
#include <memory>

namespace message_queue_system
{
   template<typename T>
   class message_queue_fine_grain_locking_resize
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
      message_queue_fine_grain_locking_resize();
      message_queue_fine_grain_locking_resize(const message_queue_fine_grain_locking_resize&) = delete;
      message_queue_fine_grain_locking_resize& operator=(const message_queue_fine_grain_locking_resize&) = delete;
      
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
   message_queue_fine_grain_locking_resize<T>::message_queue_fine_grain_locking_resize() :
   _head(std::make_unique<node_t>()), _tail(_head.get())
   {}
   
   ///
   /// @brief: check if the queue has been terminated
   ///
   template<typename T>
   bool message_queue_fine_grain_locking_resize<T>::is_terminated()
   {
      return _done.load();
   }
   
   ///
   /// @brief: set the done flag in order to terminate  the queue
   ///
   template<typename T>
   void message_queue_fine_grain_locking_resize<T>::done()
   {
      _done.store(true);
      _cv.notify_all();
   }
   
   ///
   /// @brief: pop a new message out from the queue
   ///
   template<typename T>
   bool message_queue_fine_grain_locking_resize<T>::pop(T& val)
   {
      //lock and wait for new items
      std::unique_lock<std::mutex> head_lock(_head_mutex);
      
      _cv.wait(head_lock, [&]{
         return (_done.load()) || (_head.get() != get_tail());
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
   bool message_queue_fine_grain_locking_resize<T>::try_pop(T& val)
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
   void message_queue_fine_grain_locking_resize<T>::push(T val)
   {
      lock_guard tail_lock{ _tail_mutex };
      
      if (_tail->_next)
      {
         //reuse already allocated elements
         _tail->_data = std::make_shared<T>(std::move(val));
         node_t* const new_tail = _tail->_next.get();
         _tail = new_tail;
      }
      else
      {
         auto new_data = std::make_shared<T>(std::move(val));
         std::unique_ptr<node_t> p = std::make_unique<node_t>();
         //connect new node to the list
         _tail->_data = new_data;
         node_t* const new_tail = p.get();
         _tail->_next = std::move(p);
         _tail = new_tail;
      }
      
      _cv.notify_all();
   }
   
   ///
   /// @brief: attemps to push a new element in the queue
   /// @return: true/false if the element has been pushed successfuly
   ///
   template<typename T>
   bool message_queue_fine_grain_locking_resize<T>::try_push(T val)
   {
      
      lock_t tail_lock{ _tail_mutex, std::try_to_lock };
      
      if (!tail_lock)
         return false;
      
      if (_tail->_next)
      {
         //reuse already allocated elements
         _tail->_data = std::make_shared<T>(std::move(val));;
         _tail = _tail->_next.get();
      }
      else
      {
         auto new_data = std::make_shared<T>(std::move(val));
         std::unique_ptr<node_t> p = std::make_unique<node_t>();
         //connect new node to the list
         _tail->_data = new_data;
         node_t* const new_tail = p.get();
         _tail->_next = std::move(p);
         _tail = new_tail;
      }
      
      _cv.notify_all();
      return true;
   }
   
   ///
   /// @brief: check if the queue is empty
   ///
   template<typename T>
   bool message_queue_fine_grain_locking_resize<T>::is_empty()
   {
      lock_guard heak_lock{ _head_mutex };
      return (_head.get() == get_tail());
   }
   
   ///
   /// @brief: it reserves a bunch of slots in the queue in order to avoid
   ///         expensive allocations
   /// @note: better do not call this method among different threads
   ///
   template<typename T>
   void message_queue_fine_grain_locking_resize<T>::resize(const std::size_t& n)
   {
      lock_guard lk{ _tail_mutex };
      
      auto tmp = _tail;
      for (std::size_t i = 0; i < n; ++i)
      {
         auto p = std::make_unique<node_t>();
         p->_data = std::make_shared<T>(nullptr); //no function
         tmp->_next = std::move(p); 
         node_t* const new_tail = _tail->_next.get();;
         tmp = new_tail;
      }
   }
   
}
