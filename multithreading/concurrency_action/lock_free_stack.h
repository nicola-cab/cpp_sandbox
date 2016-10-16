//
//  lock_free_stack.h
//  LockFreeQueue
//
//  Created by Nicola Cabiddu on 05/08/2016.
//  Copyright © 2016 Nicola Cabiddu. All rights reserved.
//

#ifndef lock_free_stack_h
#define lock_free_stack_h

#include <memory>
#include <atomic>


namespace lock_free {

   ///
   /// This stack implementation is not lock fr
   ///
   
   template<typename T>
   class stack {
      
   public:
      
      ~stack() {
         while( !pop() );
      }
      
      void push(const T& data)
      {
         counted_node_ptr new_node;
         new_node.ptr_ = new node(data);
         new_node.external_count_ = 1;
         new_node.ptr_->next_ = head_.load();
         while(!head_.compare_exchange_weak(new_node.ptr_->next_, new_node));
      }
      
      
      std::shared_ptr<T> pop()
      {
         counted_node_ptr old_head = head_.load();
         for(;;)
         {
            increase_head_count(old_head);
            node* const ptr = old_head.ptr_;
            if( !ptr )
            {
               return std::make_shared<T>();
            }
            
            if(head_.compare_exchange_strong(old_head,ptr->next_))
            {
               std::shared_ptr<T> res;
               res.swap(ptr->data_);
               int const count_increase = old_head.external_count_ - 2;
               
               if(ptr->internal_count.fetch_add(count_increase) == -count_increase )
               {
                  delete ptr;
               }
               
               return res;
            }
            else if(ptr->internal_count.fetch_sub(1) == 1)
            {
               //this must spin to avoid to delete some pointer that has been already deleted
               if( ptr != nullptr)
                  delete ptr;
            }
         }
      }

   private:
   
      struct node;
      
      struct counted_node_ptr {
         int external_count_ ;
         node* ptr_;
      };
      
      struct node {
         
         std::shared_ptr<T> data_;
         std::atomic<int> internal_count;
         counted_node_ptr next_;
         
         node(const T& data) :
         data_(std::make_shared<T>(data)),
         internal_count{0}
         {}
         
      };
      
      std::atomic<counted_node_ptr> head_;
      
      ///
      /// private implementation to increase the counter
      ///
      
      void increase_head_count(counted_node_ptr& old_counter)
      {
         counted_node_ptr new_counter;
         do
         {
            new_counter = old_counter;
            ++new_counter.external_count_;
         } while(!head_.compare_exchange_strong(old_counter,new_counter));
         
         old_counter.external_count_ = new_counter.external_count_;
      }
   };
   
}




#endif /* lock_free_stack_h */
