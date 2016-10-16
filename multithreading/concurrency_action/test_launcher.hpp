//
//  test_launcher.h
//  LockFreeQueue
//
//  Created by Nicola Cabiddu on 13/10/2016.
//  Copyright © 2016 Nicola Cabiddu. All rights reserved.
//

#ifndef test_launcher_hpp
#define test_launcher_hpp

#include <iostream>

namespace  util {

   namespace test_launcher
   {
      template<typename Test>
      void run(const char* name, const Test& t)
      {
         std::cout << "test: "<< name << "\n";
         t.run();
      }
   }
}



#endif /* test_launcher_hpp */
