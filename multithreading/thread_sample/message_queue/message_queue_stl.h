#pragma once

#include <queue>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <chrono>

namespace concurrency
{
    template<typename T>
    class message_queue
    {
        std::mutex _m;
        std::condition_variable _cv;
        std::queue<T> _q;
        
        using lock_guard = std::lock_guard<std::mutex>;
        
    public:
        using value_type = T;
        
        message_queue() = default;
        message_queue(const message_queue& ) = delete;
        message_queue& operator=(const message_queue&) = delete;
        
        void push_and_notify(T item)
        {
            {
                lock_guard lk{ _m };
                _q.push(std::move(item));
            }
            _cv.notify_all();
        }
        
        void push(T item)
        {
            lock_guard lk{_m};
            _q.push(std::move(item));
        }
        
        void wait_and_pop(T& item)
        {
            std::unique_lock<std::mutex> lk{ _m };
            _cv.wait(lk, [this]{ return !_q.empty(); });
            item = std::move(_q.front());
            _q.pop();
        }
        
        bool try_pop(T& t)
        {
            lock_guard lk{_m};
            if(_q.empty())
                return false;
            
            t = std::move(_q.front());
            _q.pop();
            return true;
        }
        
        bool empty()
        {
            lock_guard lk{_m };
            return _q.empty();
        }
    };
}
