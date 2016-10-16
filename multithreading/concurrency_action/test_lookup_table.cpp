//
//  test_lookup_table.cpp
//  LockFreeQueue
//
//  Created by Nicola Cabiddu on 06/10/2016.
//  Copyright © 2016 Nicola Cabiddu. All rights reserved.
//

#include "test_lookup_table.h"
#include "lookup_table.hpp"
#include <string>
#include <functional>
#include <iostream>
#include <cassert>
#include <future>
#include <iostream>


bool concurrency::test::lookup_table::run() const
{   
   concurrency::lookup_table<std::string, std::string> lt;

   auto f = [&lt]()
   {
      for(unsigned char c = 65; c < 90; ++c)
         lt.add(std::to_string(c), std::to_string(c));

   };
   
   auto f2 = [&lt]()
   {
      for(unsigned char i = 65; i < 90; ++i)
      {
         auto v = std::to_string(i);
         std::cout << "Value -> " <<  lt.get(v) << "   ";
         std::cout << "Count ->" << lt.count(v) << "   ";
         std::cout << "Bucket id ->" << lt.bucket_id(v) << "\n";
      }
      
      auto map = lt.convert_to_map();
      for(const auto& val : map)
         std::cout << val.first<< "\n";
      
   };
   
   auto r = std::async(std::launch::async, f);
   auto r1 = std::async(std::launch::async, f);
   auto r2 = std::async(std::launch::async, f2);
   
   r.get(); r1.get(); r2.get();
   
   return true;
}
