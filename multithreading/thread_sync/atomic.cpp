#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <functional>

using lock = std::lock_guard<std::mutex>;
std::mutex m;

std::atomic<double> b;
std::atomic<long> c;
std::atomic<int> d;
std::atomic<short> e;
std::atomic<char> f;
std::atomic<long long> g;

const unsigned int SIZE = 20;

int x,y;
std::atomic<int> ax{0};
std::atomic<int> ay{0};

/////
void th_read()
{
    std::cout << "x = " << x << std::endl;
    std::cout << "y = " << y << std::endl;
    std::cout << std::endl;
}
void th_write()
{
    x = 42;
    y = 20;
}

////
void th_read_lock()
{
    lock lk{m};
    
    std::cout << "x = " << x << std::endl;
    std::cout << "y = " << y << std::endl;
    std::cout << std::endl;
}
void th_write_lock()
{
    lock lk{m};
    x = 42;
    y = 20;
}

/////
void atomic_write()
{
    ax.store(42);
    ay.store(20);
}

void atomic_read()
{
    std::cout << "y = " << ay.load() << std::endl;
    std::cout << "x = " << ax.load() << std::endl;
    std::cout << std::endl;
}

//////
void atomic_read_barrier()
{
    std::cout << "y = " << ay.load() << std::endl;
    std::cout << "x = " << x << std::endl;
    std::cout << std::endl;
}

void atomic_write_barrier()
{
    x = 42;
    ay.store(20);
}


int main(int argc, char** argv)
{
    //atomic types
    std::cout << "double is atomic = "<< std::boolalpha <<  b.is_lock_free() << std::endl;
    std::cout << "long is atomic = "<< std::boolalpha <<  c.is_lock_free() << std::endl;
    std::cout << "int is atomic = "<< std::boolalpha <<  d.is_lock_free() << std::endl;
    std::cout << "short is atomic = "<< std::boolalpha <<  e.is_lock_free() << std::endl;
    std::cout << "char is atomic = "<< std::boolalpha <<  f.is_lock_free() << std::endl;
    std::cout << "long long is atomic = "<< std::boolalpha <<  g.is_lock_free() << std::endl;

    //example of use of atomics

    //neither atomics nor locks. All possible combinations are acceptable
    //std::thread t1(th_write);
    //std::thread t2(th_read);
    
    //locks (only 42 20 are acceptable values)
    //std::thread t1(th_write_lock);
    //std::thread t2(th_read_lock);

    //atomics (seq consistency)
    //std::thread t1(atomic_write);
    //std::thread t2(atomic_read);

    //atomics (barrier)
    std::thread t2(atomic_read_barrier);
    std::thread t1(atomic_write_barrier);
    
    t1.join();
    t2.join();
    return 0;
}