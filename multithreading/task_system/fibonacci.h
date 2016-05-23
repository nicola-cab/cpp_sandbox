#pragma once

#include <array>
#include <vector>

namespace computation
{
   
   namespace __guru
   {
      ///
      /// fibonacci algorithm for math gurus 
      ///

      template <typename T, typename N, typename O>
      T power(T x, N n, O op)
      {
         if (n == 0) return identity_element(op);
         while ((n & 1) == 0)
         {
            n >>= 1;
            x = op(x, x);
         }
         T result = x;
         n >>= 1;
         while (n != 0)
         {
            x = op(x, x);
            if ((n & 1) != 0) result = op(result, x);
            n >>= 1;
         }
         return result;
      }

      template <typename N>
      struct multiply_2x2
      {
         std::array<N, 4> operator()(const std::array<N, 4>& x, const std::array<N, 4>& y)
         {
            return{ x[0] * y[0] + x[1] * y[2], x[0] * y[1] + x[1] * y[3],
               x[2] * y[0] + x[3] * y[2], x[2] * y[1] + x[3] * y[3] };
         }
      };

      template <typename N>
      std::array<N, 4> identity_element(const multiply_2x2<N>&)
      {
         return{ N(1), N(0), N(0), N(1) };
      }
   }


   namespace __simple
   {
      ///
      /// fibonacci algorithm that I'd have written
      ///

      template <typename R, typename N>
      R fib(N n)
      {
         std::vector<N> cache;
         cache.assign(n + 1, 0);

         cache[0] = 1;
         cache[1] = 1;

         for (auto i = 2; i <= n; ++i)
            cache[i] = cache[i - 1] + cache[i - 2];

         return cache[n];
      }
   }

   namespace __algo
   {
      ///
      /// @brief: egyptian multiplication to compute fibonacci
      ///
      template <typename R, typename N>
      R fibonacci_math(N n)
      {
         using namespace __guru;

         if (n == 0) return R(0);
         return power(std::array<R, 4>{ 1, 1, 1, 0 }, N(n - 1), multiply_2x2<R>())[0];
      }

      ///
      /// @brief: dynamic programming to compute fibonacci
      ///
      template <typename R, typename N>
      R fibonacci_dp(N n)
      {
         using namespace __simple;
         return fib<R, N>(n);
      }
   }
  
  
   struct Algorithm_Egyptian
   {};

   struct Algorithm_DP
   {};

   ///
   /// @brief: proxy function for fibonacci
   ///
   template <typename R, typename N, typename Algo = Algorithm_Egyptian>
   R fibonacci(N n, Algo algo)
   {
      using namespace __algo;

      if(std::is_same<Algo, Algorithm_Egyptian>::value == true)
         return fibonacci_math<R, N>(n);

      return fibonacci_dp<R, N>(n);

   }

  /* template <typename R, typename N> 
   R fibonacci(N n, Algorithm_DP e)
   {
      using namespace __algo;
      return fibonacci_dp<R, N>(n);
   }*/

}



