//
//  test_lock_free_stack.cpp
//  LockFreeQueue
//
//  Created by Nicola Cabiddu on 12/08/2016.
//  Copyright © 2016 Nicola Cabiddu. All rights reserved.
//

#include "test_lock_free_stack.h"
#include "lock_free_stack.h"
#include <iostream>
#include <future>
#include <vector>
#include <cassert>

bool lock_free::test::stack::run() const
{
   lock_free::stack<char> stack;
   std::vector<char> out;
   
   auto f1 = [&stack](){
      for(int i = 100; i >0 ; --i)
         stack.push(i);
   };
   
   auto f2 = [&stack, &out]() {
      while( true ) {
         auto res = stack.pop();
         out.push_back(*res);
         if(*res == 100) break;
      }
   };
   
   auto th1 = std::async(std::launch::async, f1);
   auto th2 = std::async(std::launch::async, f2);
   th1.get();
   th2.get();
   
   assert(out.size() == 100);
   assert(!out.empty());
   
   int prev = 0;
   for(const auto& i : out)
   {
      if( prev != -1 )
         assert( prev < i );
      prev = i;
   }
   
   return true;
   
}