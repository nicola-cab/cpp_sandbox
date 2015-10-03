#include <iostream>
#include <future>
#include <chrono>
#include <algorithm>
#include <vector>

struct S
{
   long a;
   char b;
};

template<typename T>
void store_value_benchmark(T&v)
{
   std::vector<long> times;
   for(unsigned i = 0; i< 1000000; ++i)
   {
      auto b = std::chrono::high_resolution_clock::now();
      v = 10;
      auto e = std::chrono::high_resolution_clock::now();
      auto t = std::chrono::duration_cast<std::chrono::nanoseconds>(e-b).count();
      times.push_back(t);
   }
   const std::size_t size = times.size();
   std::nth_element(times.begin(), times.begin() + (size/2), times.end());
   auto median = times[size/2];
   std::cout << "Time elapsed = "<<  median << "ns\n";
}

void simple_example()
{
   S s;
   
   //update a
   auto f = [&s]()
   {
      s.a = 10;
   };
   
   //upddate b
   auto g = [&s]()
   {
      s.b = 20;
   };
   
   //print
   auto p = [&s]()
   {
      std::cout << (int)s.a << " - " << (int)s.b << std::endl;
   };
   
   
   //sample how to exploit memory model objects layout
   auto fut1 = std::async(std::launch::async,f);
   auto fut2 = std::async(std::launch::async,g);
   auto fut3 = std::async(std::launch::async,p);
   
   fut1.wait();
   fut2.wait();
   fut3.wait();

}

int main()
{
   //update different parts of object
   simple_example();
   
   //meausure I long takes update a single member
   S s;
   auto fut1 = std::async(store_value_benchmark<long>,std::ref(s.a));
   auto fut2 = std::async(store_value_benchmark<char>,std::ref(s.b));
   fut1.wait(); fut2.wait();
   
   return 0;
}