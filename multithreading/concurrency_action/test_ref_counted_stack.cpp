//
//  test_ref_counted_stack.cpp
//  LockFreeQueue
//
//  Created by Nicola Cabiddu on 04/10/2016.
//  Copyright © 2016 Nicola Cabiddu. All rights reserved.
//

#include "test_ref_counted_stack.h"
#include "ref_counted_stack.h"
#include <vector>
#include <future>
#include <cassert>

bool concurrency::test::atomic_stack::run() const
{
   concurrency::atomic_stack<char> stack;
   std::vector<char> out;
   
   auto f1 = [&stack](){
      for(int i = 100; i >0 ; --i)
         stack.push(i);
   };
   
   auto f2 = [&stack, &out]() {
      int i = 0;
      while( true ) {
         auto res = stack.pop();
         out.push_back(*res);
         if(++i == 100) break;
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
