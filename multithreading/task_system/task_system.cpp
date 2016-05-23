///
/// entry point testing task systems
///

#include<iostream>
#include <array>
#include <cassert>

#include "task_system_sched_stealing.h" 
#include "task_system_sched.h"
#include "task_system.h"
#include "timer.h"
#include "Fibonacci.h"

using computation::fibonacci;
using task_system::task_system_task_stealing_t;
using task_system::task_system_queue_thread_t;
using task_system::task_system_t;

Timer timer;
#define TIMER_START() timer.start()
#define TIMER_END()   timer.end()
#define TIMER_TIME()  timer.elapsed<Timer::us>()

//spin factor
const unsigned int SPIN_FACTOR = 100000;

//global sutff to keep track of results
unsigned int g_index = 0;
std::array<long, 4> result;

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

///
/// @brief: verify that all the fibonacci computed the same value
///
bool verify_equality(const std::array<long, 4>& result)
{
   unsigned b = 0;
   auto l = result.size() -1;
   while (b < l) 
   {
      if (result[b] != result[l])
         return false;

      ++b; --l;
   }
   return true;
}

///
/// @breif: test benchmark
///
template<typename F>
void test_bench(const F& f) {

   int64_t sum = 0;
   for (unsigned i = 0; i < SPIN_FACTOR; ++i) 
   {
      TIMER_START();
      f();
      TIMER_END();
      sum += TIMER_TIME();
   }

   std::cout << "Avg time per computation = " << sum / SPIN_FACTOR << " us\n";
   std::cout << "Total time = " << (double)sum/1000000 << " sec \n";
}

///
/// generic fibonacci
///
void fib() 
{
   result[g_index] = fibonacci<long>(500, computation::Algorithm_DP {});
}

///
/// @brief: single thread fibonacci
///
void single_thread_fib()
{ 
   test_bench(fib);   
}

///
/// @brief: launcher async operation for different task pools
///
template<typename TaskSystem>
void launcher_task_pool(TaskSystem& task_system)
{
   auto async_process = [&task_system]() {
      task_system.async(fib);
   };
   test_bench(async_process);
}


/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////


int main(int argc, char* argv[])
{
   //single thread
   std::cout << "Single thread fibonacci " << std::endl;
   single_thread_fib();

   //task pools
   task_system_t task_system_simple; 
   task_system_queue_thread_t task_system_queue;
   task_system_task_stealing_t task_system_stealing;
   std::cout << "\nTask system fibonacci with simple queue " << std::endl;
   ++g_index;
   launcher_task_pool(task_system_simple);

   std::cout << "\nTask system fibonacci with task per queue " << std::endl;
   ++g_index;
   launcher_task_pool(task_system_queue);

   std::cout << "\nTask system fibonacci with task stealing " << std::endl;
   ++g_index;
   launcher_task_pool(task_system_stealing);

   //verify that fibonacci is actually right
   assert(verify_equality(result));

	return 0;
}

