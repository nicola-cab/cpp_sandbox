//
//  test_lock_free_queue.cpp
//  LockFreeQueue
//
//  Created by Nicola Cabiddu on 19/08/2016.
//  Copyright © 2016 Nicola Cabiddu. All rights reserved.
//

#include "test_lock_free_queue.h"
#include "lock_free_queue.h"
#include <vector>
#include <iostream>
#include <future>
#include <cassert>

bool lock_free::test::queue::run() const
{
   lock_free::queue<int> queue;
   std::vector<int> out;
   int i = 0;
   
   auto f1 = [&queue](){
      for(int i = 100; i >0 ; --i)
         queue.push(i);
   };
   
   auto f2 = [&queue, &out, &i]() {
      while( true )
      {
         auto res = queue.pop();
         if(res.get() != nullptr)
         {
            out.push_back(*res);
            i += 1;
            if(*res == 1){
               assert( i == 100);
               break;
            }
         }
      }
   };
   
   auto th1 = std::async(std::launch::async, f1);
   auto th2 = std::async(std::launch::async, f2);
   th1.get();
   th2.get();
   
   int prev = -1;
   assert(out.size() == 100);
   for(const auto& v : out)
   {
      if(prev != -1)
         assert(prev > v);
      prev = v;
   }
      
   return true;
   

}