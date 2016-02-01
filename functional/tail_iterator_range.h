#pragma once

#include <iterator>
#include <string>

///
/// @brief: iterator related algorithms
///

namespace tail_recursive_stl {

   ///
   /// @brief: advance iterator
   ///
   struct advance {

      static const std::string _name;

      template<
         typename I, //models input iterator
         typename N > //number of steps to advance iterator

         I operator()(I it, N n) const
      {
         if (n == 0) return it;
         return operator()(std::next(it), n - 1);
      }
   };

   ///
   /// @brief: distance between two iterators
   ///
   struct distance {

      static const std::string _name;

      template< typename I > //models a input iterator
      typename std::iterator_traits<I>::difference_type 
         operator()(I b, I e) const
      {
         if (b == e) return 0;
         return 1 + operator()(std::next(b), e);
      }
   };

   const std::string advance::_name = "tail_recursive_stl::advance";
   const std::string distance::_name = "tail_recursive_stl::distance";

}