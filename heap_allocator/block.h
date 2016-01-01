//
//  block.h
//  Simple_Malloc
//
//  Created by Nicola Cabiddu on 13/12/2015.
//  Copyright © 2015 Nicola Cabiddu. All rights reserved.
//

#ifndef block_h
#define block_h

#include <cstddef>

//no sizeof is used here since data_ must not be taken in consideration

namespace alloc
{

   struct node_t
   {
      //number of block info hosted in this block in case of splits and merges
      unsigned char block_info_size_;
      //store number of elements allocated
      std::size_t size_;
      //prev and next poiter of my double linked list
      node_t* prev_;
      node_t* next_;
      //ptr to check that the pointer I am freeing belongs to this block
      void* ptr_;
      //flag to set status of this block (true allocated, false otherwise)
      bool free_;
      //padding info to understand the end of this block
      unsigned char data_ ;
   };
}

//sizeof won't work here because it will count also eventual padding bytes plus my char marker
#define BLOCK_SIZE 41



#endif /* block_h */
