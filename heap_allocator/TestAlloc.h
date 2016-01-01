//
//  TestAlloc.h
//  Simple_Malloc
//
//  Created by Nicola Cabiddu on 24/12/2015.
//  Copyright © 2015 Nicola Cabiddu. All rights reserved.
//

#ifndef TestAlloc_h
#define TestAlloc_h
#include "block.h"

using block_t = alloc::node_t*;

namespace alloc
{
   class alloc_test
   {
   public:
      
      //low level test routines
      void test_extend_heap();
      void test_find_block();
      void test_split_block();
      void test_fusion();
      void test_get_block();
      void test_valid_addr();
      void test_copy_block();
      
      //high level test routines
      void test_malloc();
      void test_calloc();
      void test_realloc();
      void test_free(void* p);
      
   };
}



#endif /* TestAlloc_h */
