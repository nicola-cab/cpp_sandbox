#include <thread>
#include <atomic>
#include <iostream>


class spinlock
{
    std::atomic_flag _flag;
public:
    spinlock() : _flag(ATOMIC_FLAG_INIT)
    {}
    
    spinlock(const spinlock&) = delete;
    spinlock& operator=(const spinlock&) = delete;
    spinlock(spinlock&&) = default;
    
    void lock()
    {
        while(_flag.test_and_set(std::memory_order_acquire));
    }
    
    void unlock()
    {
        _flag.clear(std::memory_order_release);
    }
};

class spinlock_guard
{
    spinlock& _spin;
public:
    spinlock_guard(spinlock& spin) : _spin(spin)
    {
        _spin.lock();
    }
    ~spinlock_guard()
    {
        _spin.unlock();
    }
};


void th_write(spinlock& spin, int& a )
{
    spinlock_guard lk{spin};
    a = 42;
}

void th_read(spinlock& spin, int& a)
{
    spinlock_guard lk{spin};
    std::cout << " value = " << a << "\n";
}


int main(int argc, char** argv)
{
    int a = 0;
    spinlock spin;
    
    std::thread th1(th_write, std::ref(spin), std::ref(a));
    std::thread th2(th_read, std::ref(spin), std::ref(a));
    
    th1.join();
    th2.join();
    return 0;
}
