#pragma once

#include <iterator>
#include <algorithm>
#include <string>

namespace tail_recursive_stl {

   struct count {

      static const std::string _name;

      template<
         typename I, //models Input iterator
         typename T //models a regular type
      >

      typename std::iterator_traits<I>::difference_type
      operator()(I b, I e, const T& val) const
      {
         if (b == e) return 0;
         return (*b == val) + operator()(std::next(b), e, val);
      }

   };

   struct count_if {

      static const std::string _name;

      template<
         typename I, //models Input iterator
         typename UnaryOp //models unary operator
      >

      typename std::iterator_traits<I>::difference_type
      operator()(I b, I e, UnaryOp op) const
      {
         if (b == e) return 0;
         return op(*b) + operator()(std::next(b), e, op);
      }

   };

   struct accumulate {

      static const std::string _name;

      template<
         typename I, //models input iterator
         typename T, //models a regual type
         typename BinaryOp //binary operation to be executed
      >
      T operator()(I b, I e, const T& init, BinaryOp op) const
      {
         if (b == e) return init;
         return operator()(std::next(b), e, op(init, *b), op);
      }

   };

   struct adjacent_difference {

      static const std::string _name;

      template<
         typename InputIt,
         typename OutputIt
      >
      OutputIt _adj_diff(
         InputIt b,
         InputIt n,
         InputIt e,
         OutputIt result) const
      {
         if (n == e) return result;
         *result = *n - *b;
         return _adj_diff(n, std::next(n), e, std::next(result));
      }

      template<
         typename InputIt,
         typename OutputIt
      >
      OutputIt operator()(InputIt b, InputIt e, OutputIt result) const
      {
         if (b == e) 
            return result;
         *result = *b;
         auto ret = result; //memorize the begin of the vector to do not loose it
         auto i = _adj_diff(b, std::next(b), e, std::next(result));
         return ret;
      }
   };

   struct inner_product {

      static const std::string _name;

      template<
         typename InputIt1, //models a input iterator
         typename InputIt2, //models a input iterator
         typename T //models semi-regular type
      >
      T operator()(InputIt1 b1, InputIt1 e1, InputIt2 b2, T init) const
      {
         if (b1 == e1) return init;
         return operator()(std::next(b1), e1, std::next(b2), init+(*b1)*(*b2));
      }
   };

   struct partial_sum {

      static const std::string _name;

      template <
         typename InputIt, //models input iterator
         typename OutputIt //models output iterator
      >
      OutputIt _partial_sum(InputIt b, InputIt e, OutputIt res, typename std::iterator_traits<OutputIt>::value_type val) const
      {
         if (b == e) 
            return res;
         return _partial_sum(std::next(b), e, std::next(res), *res = *b + val );
      }


      template <
         typename InputIt, //models input iterator
         typename OutputIt //models output iterator
      >

      OutputIt operator()(InputIt b, InputIt e, OutputIt res) const
      {
         auto tmp = res;
         _partial_sum(b, e, res, 0);
         return res;
      }

   };
}

const std::string tail_recursive_stl::count::_name = "tail_recursive_stl::count";
const std::string tail_recursive_stl::count_if::_name = "tail_recursive_stl::count_if";
const std::string tail_recursive_stl::accumulate::_name = "tail_recursive_stl::accumulate";
const std::string tail_recursive_stl::adjacent_difference::_name = "tail_recursive_stl::adjacent_difference";
const std::string tail_recursive_stl::inner_product::_name = "tail_recursive_stl::inner_product";
const std::string tail_recursive_stl::partial_sum::_name = "tail_recursive_stl::partial_sum";


