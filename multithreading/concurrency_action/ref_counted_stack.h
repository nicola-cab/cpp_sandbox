//
//  ref_counted_queue.h
//  LockFreeQueue
//
//  Created by Nicola Cabiddu on 04/10/2016.
//  Copyright © 2016 Nicola Cabiddu. All rights reserved.
//

#ifndef ref_counted_stack_h
#define ref_counted_stack_h

#include <memory>
#include <atomic>


//namespace is misleading. This implementation is not lock free.
//atomic_shared_ptr will make this implementation lock free once it gets shipped

namespace concurrency {
   
   template<typename T>
   class atomic_stack
   {
   private:
      
      struct node {
         std::shared_ptr<T> data_;
         std::shared_ptr<node> next_;
         
         node(const T& data) : data_(std::make_shared<T>(data))
         {}
      };
      
      std::shared_ptr<node> head_;
      
   public:
      
      void push(T const& data)
      {
         std::shared_ptr<node> const new_node=std::make_shared<node>(data);
         new_node->next_= std::atomic_load(&head_);
         while(!std::atomic_compare_exchange_weak(&head_,
                                                  &new_node->next_,new_node));
      }
      std::shared_ptr<T> pop()
      {
         std::shared_ptr<node> old_head=std::atomic_load(&head_);
         while(old_head && !std::atomic_compare_exchange_weak(&head_,
                                                              &old_head,old_head->next_));
         return old_head ? old_head->data_ : std::shared_ptr<T>();
      }
   };
   
}


#endif /* ref_counted_stack_h */
