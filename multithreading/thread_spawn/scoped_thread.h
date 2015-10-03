#pragma once
#include <thread>
#include <iostream>
/*
 * Scoped thread (Anthony Williams solution)
 * */
class scoped_thread
{
    std::thread t_;
public:
    explicit scoped_thread(std::thread t ) : t_(std::move(t))
    {
        if(t_.joinable() == false )
            std::logic_error("This is not a thread!!");

    }
    ~scoped_thread()
    {
		 if(t_.joinable())
		 	t_.join();
    }
    
    scoped_thread(scoped_thread&& x) : t_(std::move(x.t_))
    {}
    
    scoped_thread(scoped_thread&) = delete;
    scoped_thread& operator=(const scoped_thread&) = delete;
};
