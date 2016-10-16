//
//  ThreadSafeList.h
//  LockFreeQueue
//
//  Created by Nicola Cabiddu on 12/10/2016.
//  Copyright © 2016 Nicola Cabiddu. All rights reserved.
//

#ifndef ThreadSafeList_h
#define ThreadSafeList_h

#include <mutex>
#include <memory>

namespace  concurency {
   
   template<typename T>
   class thread_safe_list
   {
      using mutex_t = std::mutex;
      using unique_lock_t = std::unique_lock<mutex_t>;
      using lock_guard_t = std::lock_guard<mutex_t>;
      
      struct node
      {
         mutex_t m_;
         std::shared_ptr<T> data_;
         std::unique_ptr<node> next_;
         
         node() :
            next_()
         {}
         
         node(T const& value) :
            data_(std::make_shared<T>(value))
         {}
      };
      
      node head_;
      
   public:
      
      thread_safe_list()
      {}
      
      ~thread_safe_list()
      {
         //remove each node
         remove_if([](const T&){return true;});
      }
      
      thread_safe_list(const thread_safe_list&) = delete;
      thread_safe_list& operator=(const thread_safe_list&) = delete;
      
      void push_front(const T& value)
      {
         std::unique_ptr<node> new_node = std::make_unique<node>(value);
         lock_guard_t lk{head_.m_};
         new_node->next_ = std::move(head_.next_);
         head_.next_ = std::move(new_node);
      }
      
      template<typename Func>
      void for_each(Func func)
      {
         node* current = &head_;
         unique_lock_t lk{head_.m_};
         
         while( node* const next = current->next_.get())
         {
            unique_lock_t next_lk{next->m_};
            lk.unlock();
            
            func(*next->data_);
            current = next;
            lk = std::move(next_lk);
         }
      }
      
      template<typename Pred>
      std::shared_ptr<T> find_first_if(Pred pred)
      {
         node* current = head_.get();
         unique_lock_t lk{head_.m_};
         
         while( node* const next = current->next_.get())
         {
            unique_lock_t next_lk{next->m_};
            lk.unlock();
            
            if(pred(*next->data_))
            {
               return next->data_;
            }
            current = next;
            lk = std::move(next_lk);
         }
         return std::make_shared<T>();
      }
      
      template<typename Pred>
      void remove_if(Pred pred)
      {
         node* current = &head_;
         unique_lock_t lk{head_.m_};
         
         while( node* const next = current->next_.get())
         {
            unique_lock_t next_lk{next->m_};
            
            if(pred(*next->data_))
            {
               //remove
               
               std::unique_ptr<node> old_next = std::move(current->next_);
               current->next_ = std::move(next->next_);
               next_lk.unlock();
            }
            
            
            lk.unlock();
            current = next;
            lk = std::move(next_lk);
         }
      }
      
   };
   
}


#endif /* ThreadSafeList_h */
