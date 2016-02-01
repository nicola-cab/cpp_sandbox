#pragma once

///
/// @brief: list of algorithms that require as input 2 ranges
///

#include <algorithm>
#include <string>
#include <utility>

namespace stl_algorithm {

   ///
   /// @brief: mismatch algorithm
   ///
   struct mismatch {

      static const std::string _name;

      template<
         typename I1, //models Input Iterator
         typename I2 //models Input Iterator
      >

      std::pair<I1,I2>
      operator()(I1 b1, I1 e1, I2 b2) const
      {
         return std::mismatch(b1, e1, b2);
      }

   };

   ///
   /// @brief: equal algorithm
   ///
   struct equal {

      static const std::string _name;

      template<
         typename I1, //models Input Iterator
         typename I2 //models Input Iterator
      >

      bool
      operator()(I1 b1, I1 e1, I2 b2) const
      {
         return std::equal(b1, e1, b2);
      }

   };

   ///
   /// @brief: is permutation
   ///
   struct is_permutation {

      static const std::string _name;

      template<
         typename I1, //models Input Iterator
         typename I2 //models Input Iterator
      >

      bool
      operator()(I1 b1, I1 e1, I2 b2) const
      {
         return std::is_permutation(b1, e1, b2);
      }
   };

   ///
   /// @brief: search
   ///

   struct search {

      static const std::string _name;

     
      template<
         typename F1,  //models forward iterator
         typename F2  //models forward iterator
      > 

      F1 operator()(F1 b1, F1 e1, F2 b2, F2 e2) const
      {
         return std::search(b1, e1, b2, e2);
      }

   };

   const std::string mismatch::_name = "std::mismatch";
   const std::string equal::_name = "std::equal";
   const std::string is_permutation::_name = "std::is_permutation";
   const std::string search::_name = "std::search";


}

