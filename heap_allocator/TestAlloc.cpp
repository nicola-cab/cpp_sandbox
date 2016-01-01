//
//  TestAlloc.cpp
//  Simple_Malloc
//
//  Created by Nicola Cabiddu on 24/12/2015.
//  Copyright © 2015 Nicola Cabiddu. All rights reserved.
//

#include "TestAlloc.h"
#include <cassert>
#include <string>
#include <cstring>
#include "alloc.h"

namespace alloc
{
   //
   // High level interface
   //
   
   void alloc_test::test_malloc()
   {
      using namespace alloc;
      
      struct obj {
         int a;
         std::string s;
      };
      
      constexpr std::size_t N = 2 * sizeof(int);
      constexpr std::size_t M = 3 * sizeof(float);
      
      int* ptr = reinterpret_cast<int*>(alloc::malloc(N));
      float* ptr1 = reinterpret_cast<float*>(alloc::malloc(M));
      obj* ptr2 = reinterpret_cast<obj*>(alloc::malloc(sizeof(obj)));
      
      for(auto i = 0; i < 2; ++i)
      {
         ptr[i] = i;
         ptr1[i] = i+0.1;
      }
      ptr2->a = 10;
      ptr2->s = "Hello World";
      
      assert(ptr!=nullptr);
      assert(ptr1!=nullptr);
      assert(ptr2!=nullptr);
      assert(reinterpret_cast<uintptr_t>(&ptr) != reinterpret_cast<uintptr_t>(&ptr1));
      assert(reinterpret_cast<uintptr_t>(&ptr) != reinterpret_cast<uintptr_t>(&ptr2));
      assert(ptr[0] == 0);
      assert(ptr[1] == 1);
      assert(ptr1[0] == 0.1f);
      assert(ptr1[1] == 1.1f);
      assert(ptr2->a == 10);
      assert(ptr2->s == "Hello World");
      
      alloc::free(ptr);
      alloc::free(ptr1);
      alloc::free(ptr2);
   }
   
   void alloc_test::test_calloc()
   {
      constexpr std::size_t N = 10*sizeof(int);
      auto ptr = reinterpret_cast<int*>(alloc::calloc(10, sizeof(int)));
      int cmp[] = {0,0,0,0,0,0,0,0,0,0};
      auto res = std::memcmp(ptr, cmp, N);
      assert(res == 0);
      alloc::free(ptr);
   }
   
   void alloc_test::test_realloc()
   {
      constexpr std::size_t N = 10*sizeof(int);
      int* p = reinterpret_cast<int*>(alloc::malloc(N));
      assert(p!=nullptr);
      int a[N] = {1,1};
      std::memcpy(p, a, N);
      assert(std::memcmp(p, a, N) == 0);
      p = reinterpret_cast<int*>(alloc::realloc(p, N + sizeof(int)));
      assert(p!=nullptr);
      assert(std::memcmp(p, a, N) == 0);
      p[N+1] = 1;
      assert(std::memcmp(p, a, N) == 0);
      assert(p[N+1] == 1);
      alloc::free(p);
   }
   
   void alloc_test::test_free(void* p)
   {
      //todo: think about something
   }
   
   
   //
   // Low level interface
   //
   
   //
   // test routine extend heap
   //
   void alloc_test::test_extend_heap()
   {
      using namespace _util;
      
      constexpr std::size_t N = sizeof(int)*2;
      std::size_t s = align_block(N);

      block_t b = extend_heap(nullptr,s);
      
      assert(b!=nullptr);
      assert(b->free_ != true);
      assert(b->next_ == nullptr);
      assert(b->prev_ == nullptr);
      assert(b->size_ == s);
      assert(b->ptr_ == &b->data_);
            
   }
   
   //
   // test routine to find a block
   //
   void alloc_test::test_find_block()
   {
      using namespace _util;
      
      constexpr std::size_t N = sizeof(int)*2;
      std::size_t s = align_block(N);
      block_t b = extend_heap(nullptr,s);
      
      assert(b!=nullptr);
      
      block_t l = b;
      auto f = find_block(nullptr, s);
      
      assert(l!=nullptr);
      
      if( f == nullptr ) //first allocation
      {
         assert(l->next_ == b->next_);
         assert(l->prev_ == b->prev_);
         assert(l->size_ >= s);
         assert(l->ptr_ == b->ptr_);
      }
      else
      {
         assert(f->prev_ == l);
         assert(l->next_ == f);
         assert(f->size_ >= s);
         assert(f->free_ == false);
         assert(f->ptr_ == &f->data_);
      }
      
      shrink_heap(b);
   }
   
   //
   // test routine to split a block
   //
   void alloc_test::test_split_block()
   {
      using namespace _util;
      
      constexpr std::size_t N = 3 * sizeof(int);
      constexpr std::size_t M = 2 * sizeof(int);

      auto block = extend_heap(nullptr, N);
      auto size = block->size_;
      split_block(block, M);
      
      assert(block->next_->prev_ == block);
      assert(block->size_ == M);
      assert(block->next_->size_ == size-M);
      
      //release memory
      shrink_heap(block);
   }
   
   void alloc_test::test_fusion()
   {
      using namespace _util;
      constexpr std::size_t N = 4 * sizeof(int);
      constexpr std::size_t M = N + sizeof(int);
      
      
      auto b = extend_heap(nullptr,N);
      block_t head = b;
      auto b1 = extend_heap(head, M);
      
      assert(b->size_ == N);
      assert(b1->size_ == M);
      
      assert(b->next_ == b1);
      assert(b1->prev_ == b);
      
      //set b1 free
      b1->free_ = true;

      //call fusion
      fusion(b);
      assert(b->size_ == N +  M);
      assert(b->next_ == nullptr);
      assert(b->prev_ == nullptr);
      
      //release also b
      shrink_heap(b);
   }
   
   void alloc_test::test_get_block()
   {
      using namespace _util;
      
      //create a block
      constexpr std::size_t N = sizeof(int) * 4;
      block_t b = extend_heap(nullptr, N);
      
      //check block is valid
      assert(b->block_info_size_ == 1);
      assert(b->size_ == N);
      assert(b->next_ == nullptr);
      assert(b->prev_ == nullptr);
      assert(b->ptr_ == &b->data_);
      
      //try to find the block and check if this is correct
      auto ptr = get_block(b->ptr_);
      assert( ptr != nullptr);
      assert( ptr == b);
      
      //release the block
      shrink_heap(b);

   }
   
   void alloc_test::test_valid_addr()
   {
      using namespace _util;
      constexpr std::size_t N = sizeof(int) * 4;
      int* b = reinterpret_cast<int*>(alloc::malloc(N));
      assert(b!=nullptr);
      assert(valid_addr(b) == true);
      alloc::free(b);
   }
   
   void alloc_test::test_copy_block()
   {
      using namespace _util;
      constexpr std::size_t N = sizeof(int) * 4;
      constexpr std::size_t M = sizeof(int) * 6;

      block_t dest = extend_heap(nullptr, N);
      auto head = dest;
      block_t src = extend_heap(head, M);
      
      //check blocks
      assert( dest ->next_ == src);
      assert( src->prev_ == dest);
      assert( dest->size_ == N);
      assert( src->size_ == M);
      
      std::memset(src->ptr_, 0, N);
      
      auto p = reinterpret_cast<int*>(src->ptr_);
      for( unsigned i = 0; i < N; ++i)
         assert(p[i] == 0);
            
      //copy block
      copy_block(src, dest);
      
      //compare 2 blocks
      auto res = std::memcmp(src->ptr_, dest->ptr_, N);
      assert( res == 0);
      
      //release blocks
      shrink_heap(src);
      shrink_heap(dest);
   }

}