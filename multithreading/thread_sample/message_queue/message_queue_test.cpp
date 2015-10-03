#include <iostream>
#include <future>
#include <vector>
#include <chrono>
#include <mutex>
#include <thread>

#include "message_queue_stl.h"
#include "message_queue_crafted.h"
#include "test_queue.h"
#include "util.h"

using concurrency::message_queue;   // simple locking or just simple notify/wait using std::queue
using concurrency::message_queue_c; //fine grained locking 
using type_t = unsigned int;
using result_t = std::vector<type_t>;


/*
 * Producer/Consumer example
 */
template<typename Q, typename F, typename G>
std::vector<type_t> producer_consumer_test(Q& q, F& f, G& g, bool benchmarks)
{
    result_t res(N*LOOP_SIZE);
    std::vector<std::future<void>> consumers(M);
    std::vector<std::future<void>> producers(N);
    
    int i = 0;
    for (auto& p : producers)
        p = std::async(std::launch::async, f, std::ref(q), i++);

    for (auto& c : consumers)
        c = std::async(std::launch::async, g, std::ref(q), std::ref(res));

	for (auto& c : consumers)
      c.wait();

	for (auto& p : producers)
      p.wait();

    if(benchmarks)
    {
        print_benchmarks(push_time," push");
        print_benchmarks(pop_time," pop");
    }
    
    return res;
}

/*
 * Test using different version of message queue container
 */
template<typename T, typename S, typename Q>
void check_result(const T& b, const T& e, const S& res, Q& q)
{
    std::cout << "Is queue empty = "
    << std::boolalpha << q.empty() << "\n";
    
    std::cout << "Time taken to compute Producer/Consumer = "
    << std::chrono::duration_cast<TOTAL_MEASURE_UNIT>(e - b).count()<< " "
    << TOTAL_MEASURE_UNIT_STR <<"\n";
    
    auto size = N*LOOP_SIZE;
    int i =0;
    bool error = false;
    
    for(const auto& v: res)
    {
        if( i++ != v )
        {
            error = true;
            break;
        }
    }
    std::cout << "Final output is correct = " << std::boolalpha << !error << "\n";
}

int main(int argc, char* argv[])
{
    bool benchmarks = false;
    std::cout << "do you want to take benchmarks? (Yes - 1 / No - 0)\n";
    std::cin >> benchmarks;
    
    using Q = message_queue<type_t>;
    using Q1 = message_queue_c<type_t>;
    Q q;
    Q1 q1;
    
    std::cout << "\n Coarse grained locking -- \n";
    //simple solution plain lock
    //auto f = push<Q>;
    //auto g = try_pop<Q, result_t>;
    
    auto f = push_and_notify<Q>;
    auto g = wait_and_pop<Q, result_t>;
   
    //thread safe message queue using stl container
    if(benchmarks)
    {
        //simple solution plain lock
        //f = push_benchmark<Q>;
        //g = try_pop_benchmark<Q, result_t>;

        f = push_and_notify_benchmark<Q>;
        g = wait_and_pop_benchmark<Q, result_t>;
    }
    
    auto b = std::chrono::high_resolution_clock::now();
    auto res = producer_consumer_test(q, f, g, benchmarks );
    auto e = std::chrono::high_resolution_clock::now();
    check_result(b,e,res,q);

    //thread safe message queue using fine grained lock and crafted linked list
    
    std::cout << "\n Fine grained locking -- \n";
    auto f1 = push_and_notify<Q1>;
    auto g1 = wait_and_pop<Q1, result_t>;
    if(benchmarks)
    {
        f1 = push_and_notify_benchmark<Q1>;
        g1 = wait_and_pop_benchmark<Q1, result_t>;
    }
    
    //flush benchmarks
    push_time.flush();
    pop_time.flush();
    
    auto b1 = std::chrono::high_resolution_clock::now();
    auto res1 = producer_consumer_test(q1, f1, g1, benchmarks );
    auto e1 = std::chrono::high_resolution_clock::now();
    check_result(b1,e1,res1,q1);

    //push_time.print_benchmarks(SINGLE_MEASURE_UNIT_STR);
    //pop_time.print_benchmarks(SINGLE_MEASURE_UNIT_STR);
    
	return 0;
}

