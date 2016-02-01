///
///   BenchMark class utils 
///

#pragma once

#include <chrono>
#include <vector>
#include <random>
#include <string>
#include <cassert>
#include <iostream>
#include <type_traits>

namespace test
{
   struct Print 
   {

      template<typename I>       
      void print(I b, I e) 
      {
         while (b != e)
            std::cout << *b++ << " ";
      }
   };

   template<typename T>
   struct RandomRangeGenerator 
   {     
      using return_type = std::vector < T > ;

      return_type gen(const T& bottom, const T& top, std::size_t size)
      {
         static std::default_random_engine e{};
         static std::uniform_int_distribution<T> d{ bottom, top };
         
         std::vector<T> v;
         v.reserve(size);
         for (unsigned i = 0; i < size; ++i)
            v.push_back(d(e));

         return v;
      }
   };

   template<typename R> //return type
   struct Launcher 
   {
      using return_type = R;
      template<typename F, typename... Args > 
      return_type operator()(F&& f, Args&& ...args) const
      {
         return  f(std::forward<Args>(args)...);
      }

   };

}


