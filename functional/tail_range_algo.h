#pragma once

#include <string>
#include <iterator>
#include <algorithm>


///
/// @brief: list of algorithms that require as input 2 ranges
///

namespace tail_recursive_stl {


   ///
   /// @brief: mismatch
   ///
   struct mismatch {

      static const std::string _name;

       
      template<
         typename I1, //models Input Iterator
         typename I2 //models Input Iterator
      >

      ///
      /// @brief: important collection 2 is as big as collection 1
      ///
      std::pair<I1, I2>
      operator()(I1 b1, I1 e1, I2 b2) const
      {
         if (b1 == e1) return std::make_pair(b1, b2);
         if (*b1 != *b2) return std::make_pair(b1, b2);
         return operator()(std::next(b1), e1, std::next(b2));
      }

   };

   ///
   /// @brief: equal 
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
         if (b1 == e1) return true;
         if (*b1 != *b2) return false;
         return operator()(std::next(b1), e1, std::next(b2));
      }

   };

   ///
   /// @brief: is permutation
   ///
   struct is_permutation {

      static const std::string _name;


      template <
         class I1, //models Input Iterator
         class I2> //models Input Iterator

         bool
         _is_permutation(I1 b1, I1 curr, I1 e1, I2 b2, I2 e2) const
      {
         if (std::find(b1, curr, *curr) == curr)
         {
            return
               ((1 + std::count(std::next(curr), e1, *curr)) == std::count(b2, e2, *curr)) &&
               _is_permutation(b1, std::next(curr), e1, b2, e2);
         }
         
         else return _is_permutation(b1, std::next(curr), e1, b2, e2);
      }

      template<
         typename I1, //models Input Iterator
         typename I2 //models Input Iterator
      >

      bool
      operator()(I1 b1, I1 e1, I2 b2) const
      {
         
         auto start = std::mismatch(b1, e1, b2);
         if (start.first == e1) 
            return true;

         auto e2 = start.second;
         std::advance(e2, std::distance(start.first, e1));
         
         return _is_permutation(start.first, start.first, e1, start.second, e2);
      }
   };

   struct search {

      static const std::string _name;

      template<
         typename F1,  //models forward iterator
         typename F2  //models forward iterator
      >
      bool _start_with(F1 b1, F1 e1, F2 b2, F2 e2) const
      {
         if (b2 == e2) return true;
         if (b1 == e1) return false;
         return (*b1 == *b2) &&
            _start_with(std::next(b1),
            e1,
            std::next(b2),
            e2);
      }

      template<
         typename F1,  //models forward iterator
         typename F2  //models forward iterator
      >
      F1 _search(F1 b1, F1 e1, F2 b2, F2 e2) const
      {
         if (b1 == e1) return e1;
         if (_start_with(b1, e1, b2, e2))
            return b1;
         return _search(std::next(b1), e1, b2, e2);
      }

      template<
         typename F1,  //models forward iterator
         typename F2  //models forward iterator
      >

      F1 operator()(F1 b1, F1 e1, F2 b2, F2 e2) const
      {
         if (b2 == e2)
            return b1;
         return _search(b1, e1, b2, e2);
      }

   };

   const std::string mismatch::_name = "tail_recursive_stl::mismatch";
   const std::string equal::_name = "tail_recursive_stl::equal";
   const std::string is_permutation::_name = "tail_recursive_stl::is_permutation";
   const std::string search::_name = "tail_recursive_stl::search";



}