//
//  alloc.cpp
//  Simple_Malloc
//
//  Created by Nicola Cabiddu on 13/12/2015.
//  Copyright © 2015 Nicola Cabiddu. All rights reserved.
//

#include "alloc.h"
#include <cstdint>
#include <unistd.h>

#ifdef __APPLE__
#include <sys/mman.h>
#include <fcntl.h>
#include <cassert>
#include <errno.h>
#include <iostream>
#endif



block_t g_base = nullptr;

///
/// @brief: rough malloc implementation using best fit algorithm
///
void* alloc::malloc(std::size_t size)
{
   block_t b,last;
   std::size_t s = align_block(size);
   if( g_base )
   {
      //all other allocations
      last = g_base;
      b = _util::find_block(&last, s);
      if(b)
      {
         if ((b->size_ - s ) >= BLOCK_SIZE + SIZE)
         {
            _util::split_block(b,s);
         }
         b->free_ = false;
      }
      else
      {
         b = _util::extend_heap(last,s);
         if(b == nullptr )
            return nullptr;
      }
   }
   else
   {
      //fist allocation
      b = _util::extend_heap(NULL, s);
      if( b == nullptr )
         return nullptr;
      g_base = b;
   }
   return &b->data_;
}

///
/// @brief: calloc implementation using malloc underneath
///
void* alloc::calloc( std::size_t number, std::size_t size)
{
   auto N = number*size;
   std::size_t* ptr = reinterpret_cast<std::size_t*>(alloc::malloc(N));
   if( ptr != nullptr)
   {
      auto s = align_block(N)*SIZE;
      for (unsigned i=0; i<s ; i++)
         ptr[i] = 0;
   }
   return ptr;
}

///
/// @brief: realloc
///
void* alloc::realloc( void*ptr, std::size_t size)
{
   block_t b, block_new;
   void * new_ptr;
   if (ptr == nullptr )
      return alloc::malloc(size);
   
   if(_util::valid_addr(ptr))
   {
      std::size_t s = align_block(size);
      b = _util::get_block(ptr);
      if( b->size_ > size )
      {
         if( (b->size_ - s) >= (SIZE))
         {
            _util::split_block(b, s);
         }
      }
      else
      {
         //TODO: fix becaue subtractions could generate overflow
         if (b->next_ && b->next_->free_
             && (b->size_ + b->next_->size_) >= s)
         {
            _util::fusion(b);
            if (b->size_ - s >= (SIZE))
               _util::split_block(b,s);
         }
         else
         {
            
            new_ptr = alloc::malloc(s);
            if (!new_ptr)
               return nullptr;
            
            block_new = _util::get_block(new_ptr);
            _util::copy_block(b,block_new);
            alloc::free(ptr);
            return new_ptr;
         }
      }
   }
   
   return nullptr;
}

///
/// @brief: free implementation plus block fusion
///
void alloc::free(void* ptr)
{
   block_t b;
   if (_util::valid_addr(ptr))
   {
      b = _util::get_block(ptr);
      b->free_ = true;
      
      if(b->prev_ && b->prev_->free_)
      {
         b = _util::fusion(b->prev_);
      }
      
      if (b->next_ )
      {
         _util::fusion(b);
      }

      else
      {
         if(b->prev_)
            b->prev_->next_ = nullptr;
         else
            g_base = NULL;
         
         _util::shrink_heap(b);
      }
   }
}


/////
///// Utility routines pledged to be used as tools during memory allocation
/////


///
/// @brief: find a block in my list and return it
///
block_t alloc::_util::find_block( block_t* last, std::size_t size)
{
   block_t b = g_base;
   while( b && !(b->free_ && b->size_ > size))
   {
      *last = b;
      b = b->next_;
   }
   return  b;
}

///
/// @brief: extend the heap to make room for new data
///
block_t alloc::_util::extend_heap(block_t last, std::size_t size)
{
   block_t b = _util::allocate(size);
   if( b!= nullptr)
   {
      b->block_info_size_ = 1;
      b->size_ = size;
      b->next_ = nullptr;
      b->prev_ = last;
      b->ptr_ = &b->data_;
      if(last)
         last->next_ = b;
      b->free_ = false;
   }
   return b;
}

///
/// @brief: move the brk aka allocate more memory
///

block_t alloc::_util::allocate(std::size_t size)
{
   block_t b;
#ifdef __APPLE__
   b = (block_t)mmap(0, BLOCK_SIZE+size, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
#else
   b = (block_t)sbrk(0);
   if(sbrk(BLOCK_SIZE+size) == (void*)-1)
   {
      b = nullptr;
   }
#endif

   return b;
}

///
/// @brief: shrink the heap
///
void alloc::_util::shrink_heap(block_t b)
{
#if __APPLE__
   auto rc = munmap((void*)b, b->size_+ (BLOCK_SIZE * b->block_info_size_));
   assert(rc == 0);
#else
   brk(b);
#endif
}

///
/// @brief: split block in case I request very few bytes and my block is bigger
///
void alloc::_util::split_block(block_t b, std::size_t size)
{
   block_t new_block;
   new_block = reinterpret_cast<block_t>(&b->data_ + size);
   new_block->size_ = b->size_ - size ;
   new_block->next_ = b->next_;
   new_block->prev_ = b;
   new_block->free_ = true;
   new_block->ptr_ = &new_block->data_;
   new_block->block_info_size_ += 1;
  
   b->size_ = size;
   b->next_ = new_block;
   b->block_info_size_ = b->block_info_size_ == 0 ? 1 : b->block_info_size_/2;

   
   if(new_block->next_)
      new_block->next_->prev_ = new_block;
}

///
/// @brief: merge back blocks in case of multiple blocks free
///
block_t alloc::_util::fusion(block_t b)
{
   if (b->next_ && b->next_->free_)
   {
      b->size_ += b->next_->size_;
      b->next_ = b->next_->next_;
      b->block_info_size_ += 1;
      if (b->next_)
         b->next_->prev_ = b;
   }
   return (b);

}

///
/// @brief: given the pointer return the block
///
block_t alloc::_util::get_block(void* p)
{
   uintptr_t tmp = reinterpret_cast<uintptr_t>(p);
   tmp -= BLOCK_SIZE ;
   block_t b = reinterpret_cast<block_t>(tmp);
   return b;
}


///
/// @brief: check whether the pointer is valid
///
bool alloc::_util::valid_addr(void* p)
{
   if(g_base != nullptr)
   {
      if(_valid_addr(p) == true)
      {
         return (p == get_block(p)->ptr_);
      }
   }
   return false;
}

bool alloc::_util::_valid_addr(void* p)
{
   uintptr_t addr = reinterpret_cast<uintptr_t>(p);
   struct rlimit rlimit;
   auto res = getrlimit(RLIMIT_DATA, &rlimit);
   assert(res == 0);
   return (p > g_base && addr < rlimit.rlim_max);
}


///
/// @copy block of memory
///
void alloc::_util::copy_block(block_t src, block_t dst)
{
   int* sdata,*ddata;
   sdata = reinterpret_cast<int*>(src->ptr_);
   ddata = reinterpret_cast<int*>(dst->ptr_);
   
   //copy all
   auto src_size = src->size_;
   auto dst_size = dst->size_;
   for (std::size_t i=0;
        ( ( (i*SIZE) < src_size) && ( (i*SIZE) < dst_size) );
        i++)
      
      ddata[i] = sdata[i];
}

