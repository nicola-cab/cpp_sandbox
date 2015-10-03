#pragma once
#include <vector>
#include <chrono>
#include <iostream>
#include <string>
#include <algorithm>

namespace take_time
{
    using time_unit_t = std::string;
    const time_unit_t _milli_unit_str = "ms";
    const time_unit_t _micro_unit_str = "us";
    const time_unit_t _nano_unit_str = "ns";
    const time_unit_t _sec_unit_str = "s";
    
    class time
    {
    public:
        using _clock_t = std::chrono::high_resolution_clock;
        using time_point_t = std::chrono::time_point<_clock_t>;
        
        
        using _milli_unit = std::chrono::milliseconds;
        using _micro_unit = std::chrono::microseconds;
        using _nano_unit = std::chrono::nanoseconds;
        using _sec_unit  = std::chrono::seconds;
        
    private:
        time_point_t _start;
        time_point_t _end;
        std::vector<long> _benchmarks;
        
        
        template<typename Time>
        inline void compute_time(Time& t)
        {
            auto diff = _end - _start;
            t = std::chrono::duration_cast<Time>(diff);
        }
        
        
    public:
        
        time()
        {
            _benchmarks.reserve(100); //preallocate a number bigger enough to fit with measures
        }
        
        void start()
        {
            _start = std::chrono::high_resolution_clock::now();
        }
        
        void end()
        {
            _end = std::chrono::high_resolution_clock::now();
        }
        
        template<typename Time>
        inline void record()
        {
            Time p;
            compute_time<Time>(p);
            _benchmarks.push_back(p.count());

        }
        
        std::vector<long> get_benchmarks()
        {
            return _benchmarks;
        }
        
        void print_benchmarks(time_unit_t unit)
        {
            int i = 0;
            for(const auto& v: _benchmarks)
                std::cout << ++i <<")"<< " - measure took = " << v << " " << unit << "\n";
        }
        
        long mean_benchmarks()
        {
            long mean = 0.0;
            for(const auto& v: _benchmarks)
                 mean += v;
            
            return mean/_benchmarks.size();
        }
        
        long median_benchmarks()
        {
            const auto size = _benchmarks.size();
            std::nth_element(_benchmarks.begin(), _benchmarks.begin()+size/2, _benchmarks.end());
            return _benchmarks[size/2];
        }
        
        void flush()
        {
            _benchmarks.clear();
        }
    };
}

