#pragma once

#include <string>
#include <iterator>

namespace tail_recursive_stl
{
   struct find
   {
      static const std::string _name;

      template<
         typename I,     //models input iterator
         typename T>     //models the type T I want to search
      I operator()(I b, I e, const T& val) const
      {
         if (b == e) return b;
         if (*b == val) return b;

         return operator()(std::next(b), e, val);
      }
   };


   struct find_if
   {
      static const std::string _name;

      template<
         typename I,          //models Input iterator
         typename UnaryPred> //models a unary predicate 
      I operator()(I b, I e, UnaryPred pred) const
      {
         if (b == e) return b;
         if (pred(*b)) return b;
         return operator()(std::next(b), e, pred);
      }
   };

   struct find_if_not
   {
      static const std::string _name;

      template<
         typename I,          //models Input iterator
         typename UnaryPred> //models a unary predicate 
         I operator()(I b, I e, UnaryPred pred) const
      {
         if (b == e) return b;
         if (!pred(*b)) return b;
         return operator()(std::next(b), e, pred);
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
         static tail_recursive_stl::find find;

         if (b1 == e1) return e1;
         if (find(b2, e2, *b1) != e2) return b1;  
         return operator()(std::next(b1),e1,b2,e2);
      }
   };

   struct adject_find
   {
      static const std::string _name;


      template<typename I> //models a forward iterator
      I adject_find1(I b, I e) const
      {
         auto next = std::next(b);
         if (next == e) return e;
         if (*b == *next) return b;
         return adject_find1(next, e);
      }

      template<typename I>   //models a forward iterator
      I operator()(I b, I e) const
      {
         if (b == e) return e;
         return adject_find1<I>(b, e);
      }
   };

   struct min_element
   {
      static const std::string _name;

      template <
         typename FI // models forward iterator
      >
         FI _min(FI b, FI e, FI min) const
      {
         if (b == e) return min;
         return _min(std::next(b), e, (*min < *b ) ? min : b);
      }

      template<
         typename FI > //models Forward iterator
         FI operator()(FI b, FI e) const
      {
         if (b == e) return b;
         return _min(std::next(b), e, b);
      }
   };

   const std::string find::_name = "tail_recursive_stl::find";
   const std::string find_if::_name = "tail_recursive_stl::find_if";
   const std::string find_if_not::_name = "tail_recursive_stl::find_if_not";
   const std::string find_first_of::_name = "tail_recursive_stl::find_first_of";
   const std::string adject_find::_name = "tail_recursive_stl::adject_find";
   const std::string min_element::_name = "tail_recursive_stl::min_element";




}