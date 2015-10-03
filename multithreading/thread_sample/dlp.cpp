// Multithreading_.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <string>

#include <future>
#include <mutex>
#include <memory>
#include <atomic>

///////////////////////////////////////////////
//
// Worker - construction done in single thread mode
//
class SimpleWidget
{
   static std::mutex _mutex;
public:
   void hello() const
   {
      std::lock_guard<std::mutex> lk{ _mutex };
      std::cout << "Hi!!! my name is " << " - Mike" << std::endl;
   }
};
std::mutex SimpleWidget::_mutex{};

std::shared_ptr<SimpleWidget> _shared = std::make_shared<SimpleWidget>();

///////////////////////////////////////////////
//
// Worker - classical double lock check pattern
//

//mutex to protect

//bugged version (no reference counter)
class Widget
{

private:
   static Widget* _pInstance;
   static std::mutex _mutex;

   std::string _name;

private:
   explicit Widget(std::string s) : _name(std::move(s))
   {
      std::cout << "Ctor my class - my name = " << _name << std::endl;
   }
 
public:

   static Widget* Instance()
   {
      if (_pInstance == nullptr)
      {
         std::lock_guard<std::mutex> lk{ _mutex };
         if (_pInstance == nullptr)
         {
            _pInstance = new Widget("Mike");
         }
      }
      return _pInstance;
   }

   void hello() const 
   {
      std::lock_guard<std::mutex> lk{ _mutex };
      std::cout << "Hi!!! my name is " << _name<< std::endl;
   }

};

Widget* Widget::_pInstance{ nullptr };
std::mutex Widget::_mutex{};



//without bugs
class Widget_ReferenceCounter
{
private:

   static std::shared_ptr<Widget_ReferenceCounter> _pInstance;
   static std::mutex _mutex;

   std::string _name;
  
public:

   explicit Widget_ReferenceCounter(std::string s) : _name(std::move(s))
   {}

   static std::shared_ptr<Widget_ReferenceCounter> Instance()
   {
      if (_pInstance == nullptr)
      {
         std::lock_guard<std::mutex> lk{ _mutex };
         if (_pInstance == nullptr)
         {
            _pInstance = std::make_shared<Widget_ReferenceCounter>("Mike");
         }

      }
      return _pInstance;
   }

   void hello() const
   {
      std::lock_guard<std::mutex> lk{ _mutex };
      std::cout << "Hi!!! my name is " << _name << std::endl;
   }
};

std::shared_ptr<Widget_ReferenceCounter> Widget_ReferenceCounter::_pInstance{ nullptr };
std::mutex Widget_ReferenceCounter::_mutex{};




///////////////////////////////////////////////
//
// Worker - std::call_once
//

class Widget_CallOnce
{
   static std::shared_ptr<Widget_CallOnce> pInstance;
   static std::once_flag createFlag;
   static std::mutex _mutex;

   std::string _name;


public:

   explicit Widget_CallOnce(std::string s) : _name(std::move(s))
   {}

   static Widget_CallOnce* Instance()
   {
      auto build = [](){  pInstance = std::make_shared<Widget_CallOnce>("Mike"); };
      std::call_once(createFlag, build);
      return pInstance.get();
   }

   void hello() const
   {
      std::lock_guard<std::mutex> lk{ _mutex };
      std::cout << "Hi!!! my name is " << _name << std::endl;
   }

};

std::mutex Widget_CallOnce::_mutex{};
std::shared_ptr<Widget_CallOnce> Widget_CallOnce::pInstance = nullptr;
std::once_flag Widget_CallOnce::createFlag{};


///////////////////////////////////////////////
//
// Worker - Meyers singleton
//

class Widget_StaticSingleton
{
   static std::mutex _mutex;
   std::string _name;

public:

   explicit Widget_StaticSingleton(std::string s) : _name(std::move(s))
   {}

   static Widget_StaticSingleton& Instance()
   {
      static Widget_StaticSingleton pInstance("Mike");
      return pInstance;
   }

   void hello() const
   {
      std::lock_guard<std::mutex> lk{ _mutex };
      std::cout << "Hi!!! my name is " << _name << std::endl;
   }

};
std::mutex Widget_StaticSingleton::_mutex{};

///////////////////////////////////////////////
//
// Worker - Test for my singletons
//

//simple test using a global shared ptr
void test_simple()
{
   auto f = [](){ _shared->hello();  };
   auto w = std::async(std::launch::async, f);
   auto w1 = std::async(std::launch::async, f); 

   w.wait(); w1.wait();
}

//simple test using a simple double lock check pattern (it will crash due to resource that has deleted)
void test_simple_double_lock()
{
   auto f = []()
   {
      std::unique_ptr<Widget> a(Widget::Instance());
      a->hello();
   };

   auto w = std::async(std::launch::async, f);
   auto w1 = std::async(std::launch::async, f);

   w.wait();
   w1.wait();
}

//smarter way to use a double lock pattern and shared ptr
void test_double_lock_pattern()
{
   auto f = []()
   {
      auto a = Widget_ReferenceCounter::Instance();
      a->hello();
   };

   auto w = std::async(std::launch::async, f);
   auto w1 = std::async(std::launch::async, f);

   w.wait();
   w1.wait();
}

//c++11 tool std::call_once
void test_call_once()
{
   auto f = []()
   {
      auto a = Widget_CallOnce::Instance();
      a->hello();
   };

   auto w = std::async(std::launch::async, f);
   auto w1 = std::async(std::launch::async, f);

   w.wait();
   w1.wait();
}

//c++11 tool.. memory model + static variable makes this snippet of code run safe for multiple threads
void test_static_singleton()
{
   auto f = []()
   {
      auto a = Widget_StaticSingleton::Instance();
      a.hello();
   };

   auto w = std::async(std::launch::async, f);
   auto w1 = std::async(std::launch::async, f);

   w.wait(); w1.wait();
}


int main(int argc, char** argv)
{
   std::cout << "test 1 - "<< std::endl;
   test_simple();

   std::cout << "test 2 - " << std::endl;
   test_double_lock_pattern();

   std::cout << "test 3 - " << std::endl;
   test_call_once();

   std::cout << "test 4 - " << std::endl;
   test_static_singleton();

   std::cout << std::endl << "Do you want to crash?? (0/1)(Yes/No)" << std::endl;
   int c;
   std::cin >> c;
   if (c == 0)
   {
      std::cout << "test 5 - " << std::endl;
      test_simple_double_lock();
   }
   
   return 0;
}

