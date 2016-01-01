//
//  main.cpp
//  Simple_Malloc
//
//  Created by Nicola Cabiddu on 13/12/2015.
//  Copyright © 2015 Nicola Cabiddu. All rights reserved.
//

#include <iostream>
#include "TestAlloc.h"
#include "alloc.h"

using alloc::alloc_test;


int main(int argc, const char * argv[])
{
   alloc_test test;
   
   //low level interface tests
   test.test_extend_heap();
   test.test_find_block();
   test.test_split_block();
   test.test_fusion();
   test.test_get_block();
   test.test_valid_addr();
   test.test_copy_block();
   
   //high level interface tests
   test.test_malloc();
   test.test_calloc();
   test.test_realloc();
   
   std::cout << "All test passed!!" << std::endl;
   
   return 0;
}
