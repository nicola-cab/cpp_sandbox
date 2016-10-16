//
//  main.cpp
//  LockFreeQueue
//
//  Created by Nicola Cabiddu on 05/08/2016.
//  Copyright © 2016 Nicola Cabiddu. All rights reserved.
//

#include "test_lock_free_stack.h"
#include "test_lock_free_queue.h"
#include "test_ref_counted_stack.h"
#include "test_lookup_table.h"
#include "test_thread_safe_list.h"
#include "test_launcher.hpp"
#include <iostream>

int main(int argc, const char * argv[]) {
   
   util::test_launcher::run("atomic stack", concurrency::test::atomic_stack{});
   util::test_launcher::run("lock free queue", lock_free::test::queue{});
   util::test_launcher::run("lookup table",concurrency::test::lookup_table{});
   util::test_launcher::run("concurent forward list", concurency::test::thread_safe_list{});
   
   return 0;
}
