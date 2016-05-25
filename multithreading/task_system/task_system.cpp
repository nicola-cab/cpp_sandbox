// TaskSystem.cpp : Defines the entry point for the console application.
//

#include<iostream>
#include <array>
#include <cassert>

#include "task_system_sched_stealing.h"
#include "task_system_sched.h"
#include "task_system.h"
#include "timer.h"
#include "Fibonacci.h"

#include "basic_notification_queue.h"
#include "message_queue.h"
#include "message_queue_resize.h"

using computation::fibonacci;
using task_system::task_system_task_stealing_t;
using task_system::task_system_queue_thread_t;
using task_system::task_system_t;

//queue implementation
using message_queue_system::notification_queue_t;
using message_queue_system::message_queue_fine_grain_locking;
using message_queue_system::message_queue_fine_grain_locking_resize;

//task systems (simple notification queueu)
using task_system_1Q_t = task_system_t < notification_queue_t > ;
using task_system_NQ_t = task_system_queue_thread_t < notification_queue_t >;
using task_system_NQS_t = task_system_task_stealing_t < notification_queue_t > ;

//task system (with fine grain locking)
using function_t = std::function < void() > ;
using queue_t = message_queue_fine_grain_locking < function_t > ;
using task_system_1Q_fine_t = task_system_t < queue_t >;
using task_system_NQ_fine_t = task_system_queue_thread_t < queue_t >;
using task_system_NQS_fine_t = task_system_task_stealing_t < queue_t >;

//task system (with fine grain locking + reserve a bunch of slots in the queue)
using queue_reserve_t = message_queue_fine_grain_locking_resize < function_t > ;
using task_system_1Q_fine_alloc_t = task_system_t < queue_reserve_t >;
using task_system_NQ_fine_alloc_t = task_system_queue_thread_t < queue_reserve_t >;
using task_system_NQS_fine_alloc_t = task_system_task_stealing_t < queue_reserve_t >;


/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

static task_system_1Q_t  task_system_1Q;
static task_system_NQ_t  task_system_NQ;
static task_system_NQS_t task_system_NQS;

static task_system_1Q_fine_t task_system_1Q_fine;
static task_system_NQ_fine_t task_system_NQ_fine;
static task_system_NQS_fine_t task_system_NQS_fine;

static task_system_1Q_fine_alloc_t task_system_1Q_fine_alloc;
static task_system_NQ_fine_alloc_t task_system_NQ_fine_alloc;
static task_system_NQS_fine_alloc_t task_system_NQS_fine_alloc;

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////


Timer timer;

#define TIMER_START()  timer.start()
#define TIMER_END() timer.end()
#define TIMER_TIME() timer.elapsed<Timer::ns>()

const unsigned int SPIN_FACTOR = 100000;

unsigned int g_index = 0;
std::array<long, 4> result;

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

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
   
   std::cout << sum / SPIN_FACTOR << " ns\n";
   std::cout << "Total time = " << (double)sum / 1000000000 << " sec \n";
}

///
/// generic fibonacci
///
void fib()
{
   result[g_index] = fibonacci<long>(500, computation::Algorithm_Egyptian {});
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

void coarse_locking_fibonacci_test()
{
   //task pools (using simple notification queue)
   std::cout << "Fibonacci + task system using coarse locking queue " << std::endl;
   
   std::cout << "\nTask system 1Q " << std::endl;
   ++g_index;
   launcher_task_pool(task_system_1Q);
   
   std::cout << "\nTask system NQ " << std::endl;
   ++g_index;
   launcher_task_pool(task_system_NQ);
   
   std::cout << "\nTask system NQ work stealing " << std::endl;
   ++g_index;
   launcher_task_pool(task_system_NQS);
   
   //verify that fibonacci is actually right
   assert(verify_equality(result));
}


void fine_locking_fibonacci_test()
{
   std::cout << "\n\n Fibonacci + task system using fine locking queue " << std::endl;
   
   //reset results
   result.data()[1] = 0;
   result.data()[2] = 0;
   result.data()[3] = 0;
   g_index = 1;
   
   std::cout << "\nTask system 1Q " << std::endl;
   launcher_task_pool(task_system_1Q_fine);
   
   std::cout << "\nTask system NQ " << std::endl;
   ++g_index;
   launcher_task_pool(task_system_NQ_fine);
   
   std::cout << "\nTask system NQS work stealing " << std::endl;
   ++g_index;
   task_system_NQS_fine.resize(10);
   launcher_task_pool(task_system_NQS_fine);
   
   //verify that fibonacci is actually right
   assert(verify_equality(result));
}

void fine_locking_alloc_fibonacci_test()
{
   std::cout
   << "\n\n Fibonacci + task system using fine locking queue + allocating a bunch of slots in the queue" << std::endl;
   
   //reset results
   result.data()[1] = 0;
   result.data()[2] = 0;
   result.data()[3] = 0;
   g_index = 1;
   
   std::cout << "\nTask system 1Q " << std::endl;
   launcher_task_pool(task_system_1Q_fine);
   
   std::cout << "\nTask system NQ " << std::endl;
   ++g_index;
   launcher_task_pool(task_system_NQ_fine);
   
   std::cout << "\nTask system NQ work stealing " << std::endl;
   ++g_index;
   task_system_NQS_fine_alloc.resize(10); //10 slots reserved per queue per thread
   launcher_task_pool(task_system_NQS_fine_alloc);
   
   //verify that fibonacci is actually right
   assert(verify_equality(result));
}

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
   //single thread
   std::cout << "Single thread fibonacci " << std::endl;
   single_thread_fib();
   
   //coarse locking
   coarse_locking_fibonacci_test();
   
   //fine locking
   fine_locking_fibonacci_test();
   
   //fine locking plus reserving space first
   fine_locking_alloc_fibonacci_test();
   
   return 0;
}

