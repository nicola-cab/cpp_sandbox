//
//  test_thread_safe_list.cpp
//  LockFreeQueue
//
//  Created by Nicola Cabiddu on 12/10/2016.
//  Copyright © 2016 Nicola Cabiddu. All rights reserved.
//

#include "test_thread_safe_list.h"
#include "thread_safe_list.hpp"
#include <future>
#include <iostream>

bool concurency::test::thread_safe_list::run() const
{
   concurency::thread_safe_list<int> l_th_list;
   
   auto f1 = [&l_th_list](){
      
      for(unsigned i = 0; i < 10; ++i)
         l_th_list.push_front(i);
      
   };
   
   auto f2 = [&l_th_list]() {
      for(unsigned i = 0; i < 10; ++i)
         l_th_list.push_front(i);
   };
   
   auto f3 = [&l_th_list]() {
      auto print = [](const auto& x) {
         std::cout << x << " ";
      };
      
      l_th_list.for_each(print);
   };
   
   auto producer1 = std::async(std::launch::async, f1);
   auto producer2 = std::async(std::launch::async, f2);
   auto consumer  = std::async(std::launch::async, f3);
   
   producer1.get();
   producer2.get();
   consumer.get();
   
   return true;
}
