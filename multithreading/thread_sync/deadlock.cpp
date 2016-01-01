#include <iostream>
#include <mutex>
#include <thread>
#include <future>
#include <vector>

//recursive lock needed since I need to lock the same lock more than once per thread
using lock = std::recursive_mutex;
using lock_guard = std::lock_guard<lock>;

/*
 * Account class
 * */
class Account
{
    double _balance;
    mutable lock _mutex;
    
public:
    
    Account() : _balance(0.0)
    {}
    
    void withdraw(double amount)
    {
        lock_guard lk{_mutex};
        _balance -= amount;
    }
    
    void deposit(double amount)
    {
        lock_guard lk{_mutex};
        _balance += amount;
    }
    
    double balance() const
    {
        lock_guard lk{_mutex};
        return _balance;
    }
    
    lock& get_mutex()
    {
        return _mutex;
    }
};

/*
 * Bank class
 **/
class Bank
{
    Account _accounts[2];
public:
    Bank(double min_balance)
    {
        _accounts[0].deposit(min_balance);
    }
    
    double get_balance( int id )
    {
        return _accounts[id].balance();
    }
    
    void bad_transfer(int from, int to, int sum)
    {
        // deadlock
        auto& acc1 = _accounts[from];
        auto& acc2 = _accounts[to];
        
        lock_guard lk1(acc1.get_mutex());
        lock_guard lk2(acc2.get_mutex());
        
        std::cout << "Moving money from = " << from << " to = " << to << " sum = " << sum << "\n";
        
        if (acc1.balance() >= sum)
        {
            acc1.deposit(-sum);
            acc2.deposit(sum);
        }
    }
    
    void transfer(int from, int to, int sum)
    {
        auto& acc1 = _accounts[from];
        auto& acc2 = _accounts[to];
        
        std::lock(acc1.get_mutex(), acc2.get_mutex());
        lock_guard lk1(acc1.get_mutex(), std::adopt_lock);
        lock_guard lk2(acc2.get_mutex(), std::adopt_lock);
        
        std::cout << "Moving money from = " << from << " to = " << to << " sum = " << sum << "\n";
        
        if (acc1.balance() >= sum)
        {
            acc1.deposit(-sum);
            acc2.deposit(sum);
        }
    }
};

int main(int argc, char** argv)
{
    Bank b(100.0);
    
    auto f = [&b](int from,int to, double sum){
        b.bad_transfer(from, to, sum);
    };
    
    auto g = [&b](int from,int to, double sum) {
        b.transfer(from, to, sum);
    };
    
    std::vector<std::future<void>> tasks;
    
//    for(unsigned i = 0; i < 2; ++i)
//    {
//        tasks.push_back(std::async(f,0,1,10)); //deadlock
//        tasks.push_back(std::async(f,1,0,10));
//    }
    
    
    for(unsigned i = 0; i < 1; ++i)
    {
        tasks.push_back(std::async(g,0,1,10)); //no deadlock
        tasks.push_back(std::async(g,1,0,10));
    }
    
    for(auto& t : tasks) t.wait();
    
    std::cout << "Program end ... Balance 0 = " << b.get_balance(0)
    << " - Balance 1 = " << b.get_balance(1) << "\n";
    
    return 0;
}
