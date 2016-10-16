//
//  lock_free_queue.h
//  LockFreeQueue
//
//  Created by Nicola Cabiddu on 17/08/2016.
//  Copyright © 2016 Nicola Cabiddu. All rights reserved.
//

#ifndef lock_free_queue_h
#define lock_free_queue_h

#include <atomic>
#include <memory>
#include <cassert>
#include <iostream>

namespace lock_free
{
   template<typename T>
   class queue
   {
      
   private:
      
      struct node;
      struct counted_node_ptr
      {
         int external_count_;
         node* ptr_;
      };

      struct node_counter
      {
         unsigned internal_count_:30;
         unsigned external_counters_:2;
      };
      
      struct node
      {
         std::atomic<T*> data_;
         std::atomic<node_counter> count_;
         std::atomic<counted_node_ptr> next_;
         
         node( T v ) : node( new T( v ) )
         {}
         
         template <typename... Args>
         node( Args&&... args ) : node( T( std::forward<Args>( args )... ) )
         {}
         
         node() : node( (T*) nullptr )
         {}
         
         node( T * v ) : data_( v )
         {
            node_counter new_count;
            new_count.internal_count_ = 0;
            new_count.external_counters_ = 2;
            count_.store( new_count );
            counted_node_ptr new_next;
            new_next.ptr_ = nullptr;
            new_next.external_count_ = 0;
            next_.store( new_next );
         }
         
      };
      
      std::atomic<counted_node_ptr> head_;
      std::atomic<counted_node_ptr> tail_;
      
   public:
      
      queue()
      {
         counted_node_ptr new_head;
         new_head.ptr_ = new node;
         new_head.external_count_ = 1;
         head_.store( new_head );
         tail_.store( head_.load() );
      }
      
      ~queue()
      {
         clear();
         assert( head_.load().ptr_ == tail_.load().ptr_ );
         free_node( head_.load().ptr_ );
      }
      
      queue(const queue&) = delete;
      queue& operator=(const queue&) = delete;
      
      bool empty() const
      {
         return head_.load().ptr == tail_.load().ptr;
      }
      
      void push(T new_value)
      {
         std::unique_ptr<T> new_data = std::make_unique<T>(new_value);
         counted_node_ptr new_next{1, new node};
         counted_node_ptr old_tail=tail_.load();
         
         for(;;)
         {
            increase_external_count(tail_,old_tail);
            T* old_data=nullptr;
            if(old_tail.ptr_->data_.compare_exchange_strong(old_data,new_data.get()))
            {
               counted_node_ptr old_next={0};
               if(!old_tail.ptr_->next_.compare_exchange_strong(old_next,new_next))
               {
                  delete new_next.ptr_;
                  new_next=old_next;
               }
               set_new_tail(old_tail, new_next);
               new_data.release();
               break;
            }
            else
            {
               counted_node_ptr old_next;
               if(old_tail.ptr_ != nullptr && old_tail.ptr_->next_.compare_exchange_strong(old_next,new_next))
               {
                  old_next=new_next;
                  new_next.ptr_=new node;
               }
               set_new_tail(old_tail, old_next);
            }
         }
      }
      
      
      std::unique_ptr<T> pop()
      {
         counted_node_ptr old_head=head_.load(/*std::memory_order_relaxed*/);
         for(;;)
         {
            increase_external_count(head_,old_head);
            node* const ptr=old_head.ptr_;
            if(ptr==tail_.load().ptr_)
            {
               return std::unique_ptr<T>();
            }
            counted_node_ptr next=ptr->next_.load();
            if(head_.compare_exchange_strong(old_head,next))
            {
               T* const res=ptr->data_.exchange(nullptr);
               free_external_counter(old_head);
               return std::unique_ptr<T>(res);
            }
            release_ref(ptr);
         }
      }
      
   private:
      
      void clear()
      {
         while( pop() );
      }

      static void free_node( node * p )
      {
         delete p;
      }

      static void increase_external_count(std::atomic<counted_node_ptr>& counter, counted_node_ptr& old_counter)
      {
         counted_node_ptr new_counter;
         do
         {
            new_counter = old_counter;
            ++new_counter.external_count_;
         } while (!counter.compare_exchange_strong(old_counter, new_counter,
                                                   std::memory_order_acquire, std::memory_order_relaxed));
         
         old_counter.external_count_ = new_counter.external_count_;
      }
      
      static void free_external_counter( counted_node_ptr& old_node_ptr )
      {
         node * const ptr = old_node_ptr.ptr_;
         int const count_increase = old_node_ptr.external_count_ - 2;
         
         node_counter old_counter = ptr->count_.load( std::memory_order_relaxed );
         node_counter new_counter;
         
         do
         {
            new_counter = old_counter;
            --new_counter.external_counters_;
            new_counter.internal_count_ += count_increase;
         } while (!ptr->count_.compare_exchange_strong(old_counter, new_counter,
                                                        std::memory_order_acquire,
                                                       std::memory_order_relaxed));
         
         if ( !new_counter.internal_count_ && !new_counter.external_counters_ )
            free_node( ptr );
      }
      
      static void release_ref( node * p )
      {
         node_counter old_counter = p->count_.load( std::memory_order_relaxed );
         node_counter new_counter;
         do
         {
            new_counter = old_counter;
            --new_counter.internal_count_;
         } while (!p->count_.compare_exchange_strong(old_counter, new_counter,
                                                      std::memory_order_acquire,
                                                      std::memory_order_relaxed));
         
         if ( !new_counter.internal_count_ && !new_counter.external_counters_ )
            free_node( p );
      }
      
      void set_new_tail( counted_node_ptr& old_tail, counted_node_ptr const& new_tail )
      {
         node * const current_tail_ptr = old_tail.ptr_;
         while ( !tail_.compare_exchange_weak( old_tail, new_tail ) && old_tail.ptr_ == current_tail_ptr );
         if ( old_tail.ptr_ == current_tail_ptr )
            free_external_counter( old_tail );
         else
            release_ref( current_tail_ptr );
      }
      
      struct node_disposer
      {
         void operator()( node * p )
         {
            free_node( p );
         }
      };
   };
}

#endif /* lock_free_queue_h */
