#pragma once

#include <iostream>
#include <string>
#include <chrono>
#include <mutex>
#include "message_queue_stl.h"
#include "message_queue_crafted.h"
#include "util.h"

using concurrency::message_queue;
using concurrency::message_queue_c;

using lock_guard = std::lock_guard<std::mutex>;
std::mutex m_pop;
std::mutex m_push;

/*
 * Test routines
 */

/*
 * simple push
 * */
template<typename Q>
void push(Q& q, unsigned id)
{
    for(unsigned i=id*LOOP_SIZE; i<(id+1)*LOOP_SIZE; ++i)
        q.push(i);
}

/*
 * simple push benchmarked
 * */
template<typename Q>
void push_benchmark(Q& q, unsigned id)
{
    for(unsigned i=id*LOOP_SIZE; i<(id+1)*LOOP_SIZE; ++i)
    {
        lock_guard lk{m_push};
        
        push_time.start();
        q.push(i);
        push_time.end();
        push_time.record<SINGLE_MEASURE_UNIT>();
    }
}

/*
 * push and notify
 * */
template<typename Q>
void push_and_notify(Q& q, unsigned id)
{
    for(unsigned i=id*LOOP_SIZE; i<(id+1)*LOOP_SIZE; ++i)
        q.push_and_notify(i);
}

/*
 * push and notify benchmarked
 */
template<typename Q>
void push_and_notify_benchmark(Q& q, unsigned id)
{
    for(unsigned i=id*LOOP_SIZE; i<(id+1)*LOOP_SIZE; ++i)
    {
        lock_guard lk{m_push};
        
        push_time.start();
        q.push_and_notify(i);
        push_time.end();
        
        push_time.record<SINGLE_MEASURE_UNIT>();
    }
}

/*
 * simple try pop (melt cpu)
 * */
template<typename Q, typename S>
void try_pop(Q& q, S& res)
{
    typename Q::value_type i;
    while(true)
    {
        q.try_pop(i);
        res[i]=i;
    }
}

/*
 * simple try pop read data + benchmarks
 * */
template<typename Q, typename S>
void benchmark_read_data_try_pop(Q& q, S& res)
{
    typename Q::value_type i;
    lock_guard lk{m_pop};
    
    pop_time.start();
    q.try_pop(i);
    pop_time.end();
    
    pop_time.record<SINGLE_MEASURE_UNIT>();
    res[i]=i;
}

/*
 * simple wait and pop read routine + benchmarks
 * */
template<typename Q, typename S>
void benchmark_read_data_wait_and_pop(Q& q, S& res)
{
    typename Q::value_type i;
    
    lock_guard lk{m_pop};
    
    pop_time.start();
    q.wait_and_pop(i);
    pop_time.end();
    
    pop_time.record<SINGLE_MEASURE_UNIT>();
    res[i]=i;
}


/*
 * try pop routine with logic to extract benchmarks and ensure to read all data
 * */
template<typename Q, typename S>
void try_pop_benchmark(Q& q, S& res)
{
    
    //compute number of item to read and possible items left after main read loop
    const size_t all_data = N*LOOP_SIZE;
    const size_t block = all_data / M;
    const auto rest = all_data % M;
    
    for(unsigned i =0; i<block; ++i)
    {
        benchmark_read_data_try_pop(q,res);
    }
    
    if(rest)
    {
        while(!q.empty())
        {
            benchmark_read_data_try_pop(q,res);
        }
    }
}

/*
 * wait and pop routine (infinite loop won't melt the cpu)
 * */
template<typename Q, typename S>
void wait_and_pop(Q& q, S& res)
{
    typename Q::value_type i;

    while(true)
    {
        q.wait_and_pop(i);
        res[i] = i;
    }
}

/*
 * wait and pop routine with logic to extract benchmarks and ensure to read all data
 * */
template<typename Q, typename S>
void wait_and_pop_benchmark(Q& q, S& res)
{
    //compute number of item to read and possible items left after main read loop
    const size_t all_data = N*LOOP_SIZE;
    const size_t block = all_data / M;
    const auto rest = all_data % M;
    
    for(unsigned i =0; i<block; ++i)
    {
        benchmark_read_data_wait_and_pop(q,res);
    }
    
    if( rest )
    {
        while(!q.empty())
        {
            benchmark_read_data_wait_and_pop(q,res);
        }
    }
}

/*
 * print results
 * */
void print_benchmarks(take_time::time& t, std::string op )
{
    auto median = t.median_benchmarks();
    std::cout << "Median to " << op << " single element from message queue = " << median
    << SINGLE_MEASURE_UNIT_STR << "\n";
}
