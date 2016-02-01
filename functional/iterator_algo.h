#pragma 

#include <iterator>
#include <string>

///
/// @brief: iterator related algorithms
///

namespace stl_algorithm {

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
         //std::advance does not compile here... I got an ungly message saying that I attempted to cast a void to iterator... :-(
         while (n)
         {
            ++it;
            --n;
         }
         //return std::advance(b, n);
         return it;
      }
   };

   ///
   /// @brief: distance between two iterators
   ///
   struct distance {

      static const std::string _name;

      template< typename I >  //models a input iterator
      typename std::iterator_traits<I>::difference_type
         operator()(I b, I e) const
      {
         return std::distance(b, e);
      }
   };

   const std::string advance::_name = "stl_algorithm::advance";
   const std::string distance::_name = "stl_algorithm::distance";



}