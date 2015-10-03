#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>

namespace concurrency
{
    template<typename T>
    class message_queue_c
    {
    private:
        
        struct node_t {
            std::shared_ptr<T> _data;
            std::unique_ptr<node_t> _next;
        };
        
        std::unique_ptr<node_t> _head;
        node_t* _tail;
        std::mutex _head_mutex;
        std::mutex _tail_mutex;
        std::condition_variable _cv;
        
        using lock_guard = std::lock_guard<std::mutex>;
        
    private:
        
        node_t* get_tail()
        {
            lock_guard lk{_tail_mutex};
            return _tail;
        }
        
    public:
        
        using value_type = T;
        
        message_queue_c();
        message_queue_c(const message_queue_c& ) = delete;
        message_queue_c& operator=(const message_queue_c&) = delete;
        
        void push_and_notify(T val);
        void wait_and_pop(T& val);
        bool empty();
    };
}

template<typename T>
concurrency::message_queue_c<T>::message_queue_c() :
_head(new node_t), _tail(_head.get())
{}

template<typename T>
void concurrency::message_queue_c<T>::push_and_notify(T val)
{
    //copy data and build a new node
    auto new_data = std::make_shared<T>(std::move(val));
    std::unique_ptr<node_t> p(new node_t);
    {
        //lock tail
        lock_guard lk{_tail_mutex};
        //connect new node to the list
        _tail->_data = new_data;
         node_t* const new_tail = p.get();
        _tail->_next = std::move(p);
        _tail = new_tail;
    }
    _cv.notify_all();
}

template<typename T>
void concurrency::message_queue_c<T>::wait_and_pop(T& val)
{
    //lock and wait for new items
    std::unique_lock<std::mutex> head_lock(_head_mutex);
    _cv.wait(head_lock,[&]{return _head.get() != get_tail();});
    //extract data
    val = std::move(*_head->_data);
    //delete the head
    std::unique_ptr<node_t> old_head=std::move(_head);
    _head=std::move(old_head->_next);
}

template<typename T>
bool concurrency::message_queue_c<T>::empty()
{
    lock_guard lk{_head_mutex};
    return (_head.get() ==get_tail());
}
