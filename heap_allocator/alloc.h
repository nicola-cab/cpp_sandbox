//
//  alloc.h
//  Simple_Malloc
//
//  Created by Nicola Cabiddu on 13/12/2015.
//  Copyright © 2015 Nicola Cabiddu. All rights reserved.
//

#ifndef alloc_h
#define alloc_h


#include "block.h"
#include <cstddef>
#include <sys/resource.h>

//align data allocated.
//Laid it down in chucks that are multiple of 8bytes
#define SIZE 8
#define align_block(x) (((((x)-1)/SIZE)*SIZE)+SIZE)

using block_t = alloc::node_t*;

namespace alloc
{
   void* malloc( std::size_t size );
   void* calloc( std::size_t number, std::size_t size);
   void* realloc( void*ptr, std::size_t size);
   void free(void* ptr);

   
   namespace _util
   {
      block_t find_block( block_t* last, std::size_t size);
      block_t extend_heap(block_t last, std::size_t size);
      block_t allocate(std::size_t size);
      void shrink_heap(block_t block);
      void split_block(block_t b, std::size_t size);
      
      block_t fusion(block_t);
      block_t get_block(void*);
      bool valid_addr(void*);
      void copy_block(block_t src, block_t dst);
      
      bool _valid_addr(void* ptr);
      
   }
}


#endif /* alloc_h */
