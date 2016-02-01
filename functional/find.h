#pragma once

#include <algorithm>
#include <string>


namespace stl_algorithm
{
   struct find
   {
      static const std::string _name;

      template<
         typename I,     //models a input iterator
         typename T>     //models the type T I want to search
      I operator()(I b, I e, const T& val) const
      {
         return std::find(b, e, val);
      }
   };

   struct find_if
   {
      static const std::string _name;

      template<
         typename I,             //models a input iterator
         typename UnaryPred>     //models the type T I want to search
      I operator()(I b, I e, UnaryPred pred) const
      {
         return std::find_if(b, e, pred);
      }
   };

   struct find_if_not
   {
      static const std::string _name;

      template<
         typename I,             //models a input iterator
         typename UnaryPred>     //models the type T I want to search
         I operator()(I b, I e, UnaryPred pred) const
      {
         return std::find_if_not(b, e, pred);
      }
   };

   struct find_first_of
   {
      static const std::string _name;

      template<
         typename FI1,   //models a forward iterator
         typename FI2>   //models a forward iterator      
      FI1 operator()(FI1 b1, FI1 e1, FI2 b2, FI2 e2) const
      {
         return std::find_first_of(b1,e1,b2,e2);
      }
   };

   struct adject_find
   {
      static const std::string _name;

      template<
         typename I>   //models a forward iterator
         I operator()(I b, I e) const
      {
         return std::adjacent_find(b, e);
      }
   };

   struct min_element
   {
      static const std::string _name;

      template<
         typename FI > //models Forward iterator
         FI operator()(FI b, FI e) const
      {
         return std::min_element(b, e);
      }
   };

   const std::string find::_name = "std::find";
   const std::string find_if::_name = "std::find_if";
   const std::string find_if_not::_name = "std::find_if_not";
   const std::string find_first_of::_name = "std::find_first_of";
   const std::string adject_find::_name = "std::adject_find";
   const std::string min_element::_name = "std::min_element";


}