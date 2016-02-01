#pragma once

#include <iterator>
#include <algorithm>
#include <numeric>
#include <string>

namespace stl_algorithm
{
   struct count {

      static const std::string _name;

      template<
         typename I, //models Input iterator
         typename T //models a regular type
      >

      typename std::iterator_traits<I>::difference_type
      operator()(I b, I e, const T& val) const
      {
         return std::count(b, e, val);
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
         return std::count_if(b, e, op);
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
         return std::accumulate(b, e, init, op);
      }

   };

   struct adjacent_difference {

      static const std::string _name;

      template<
         typename InputIt, //models a input iterator
         typename OutputIt //models a output iterator
      >
      OutputIt operator()(InputIt b, InputIt e, OutputIt result) const
      {
         std::adjacent_difference(b, e, result);
         return result;
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
         return std::inner_product(b1, e1, b2, init);
      }
   };

   struct partial_sum {

      static const std::string _name;

      template <
         typename InputIt,
         typename OutputIt
      >

      OutputIt operator()(InputIt b1, InputIt e1, OutputIt res) const
      {
         std::partial_sum(b1, e1, res);
         return res;
      }

   };

}

const std::string stl_algorithm::count::_name = "stl_algorithm::count";
const std::string stl_algorithm::count_if::_name = "stl_algorithm::count_if";
const std::string stl_algorithm::accumulate::_name = "stl_algorithm::accumulate";
const std::string stl_algorithm::adjacent_difference::_name = "stl_algorithm::adjacent_difference";
const std::string stl_algorithm::inner_product::_name = "stl_algorithm::inner_product";
const std::string stl_algorithm::partial_sum::_name = "stl_algorithm::partial_sum";