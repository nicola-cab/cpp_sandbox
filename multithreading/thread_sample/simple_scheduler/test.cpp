#include "task_system.h"
#include <mutex>
#include <thread>
#include <chrono>
#include <iostream>

std::mutex m;

void exec()
{
   std::lock_guard<std::mutex> lk{m};
   std::cout << std::this_thread::get_id() << std::endl;
}

void run()
{
   task_system_t task_system;

   for(unsigned i = 0; i < 8; ++i)
      task_system.async(exec);
   
}

int main(int argc, char** argv)
{
   
   auto b = std::chrono::high_resolution_clock::now();
   run();
   auto e = std::chrono::high_resolution_clock::now();
   auto t = std::chrono::duration_cast<std::chrono::microseconds>(e-b).count();
   std::cout << "time elapsed = " << t << " us "<< std::endl;

   return 0;
}