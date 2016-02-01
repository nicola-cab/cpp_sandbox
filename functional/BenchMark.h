#pragma once

#include <iostream>
#include <cassert>
#include <string>
#include <chrono>
#include <utility>

namespace benchmark {

   template<
      typename L,         //Launcher
      typename F1,       //stl algo
      typename F2,       //tail recursive algo
      typename... Args> //list of arguments

   std::pair<typename L::return_type, typename L::return_type> 
   _run(const L& l, F1&& f1, F2&& f2, Args&&... args)
   {
      using time_frame_t = std::chrono::nanoseconds;
      const std::string unit = "ns";

      //stl algorithm
      auto b1 = std::chrono::system_clock::now();
      auto r1 = l(std::forward<F1>(f1), std::forward<Args>(args)...);
      auto e1 = std::chrono::system_clock::now();

      //tail recursive one
      auto b2 = std::chrono::system_clock::now();
      auto r2 = l(std::forward<F2>(f2), std::forward<Args>(args)...);
      auto e2 = std::chrono::system_clock::now();

      //print time taken to run both algorithms
      std::cout << "Algorithm " << f1._name
         << " has taken = " << std::chrono::duration_cast<time_frame_t>(e1 - b1).count()
         << unit << std::endl;

      std::cout << "Algorithm " << f2._name
         << " has taken = " << std::chrono::duration_cast<time_frame_t>(e2 - b2).count()
         << unit << std::endl;

      return std::make_pair(r1, r2);
   }

   struct SimpleRunner 
   {
      ///
      /// @brief: compare bools returned by the algorithms 
      ///
      void verify_result(const std::string& msg, bool i, bool ii) const
      {
         assert(i == ii);
         std::cout << "Comparison for --> " << msg << " -- " << std::boolalpha << (i == ii) << "\n\n";
      }

      ///
      /// @brief: compare integers returned by the algorithms 
      ///
      void verify_result(const std::string& msg, int i, int ii) const
      {
         assert(i == ii);
         std::cout << "Comparison for --> " << msg << " -- " << std::boolalpha << (i == ii) << "\n\n";
      }

      ///
      /// @brief: compare iterators returned by the algorithms
      ///
      template<typename I> //models input iterator
      void verify_result(const std::string& msg, I i, I ii) const
      {
         std::cout << "Comparison for --> " << msg << " -- " << std::boolalpha << (&*i == &*ii) << " and value i = "  << *i << " - ii = "<< *ii<< "\n\n";
         assert(&(*i) == &(*ii) || *i == *ii);
      }

      ///
      /// @brief: run interface
      ///
      template<
         typename L,         //Launcher
         typename F1,       //stl algo
         typename F2,       //tail recursive algo
         typename... Args> //list of arguments
         void operator()(L&& l, F1&& f1, F2&& f2, Args&&... args ) const
      {
         auto ret = _run(std::forward<L>(l), std::forward<F1>(f1), std::forward<F2>(f2), std::forward<Args>(args)...);
         verify_result(
            f1._name + " - " + f2._name,
            ret.first, 
            ret.second);
      }

   };

   struct SimpleRunnerFaultTollerante
   {

      ///
      /// @brief: compare iterators returned by the algorithms
      ///
      template<typename I> //models input iterator
      void verify_result(const std::string& msg, I i, I ii, I endi, I endii) const
      {
         std::cout << "\n\n";
        //TODO: understand why this crashes
        // std::cout << "Comparison for --> " << msg << " -- iterators both point to end " << std::boolalpha << (i == endi && ii == endii) << "/n/n";
      }

      template<
         typename L,         //Launcher
         typename F1,       //stl algo
         typename F2,       //tail recursive algo
         typename I,       //end iterator
         typename... Args> //list of arguments
         void operator()(L&& l, F1&& f1, F2&& f2, I end1, I end2, Args&&... args) const
      {
         auto ret = _run(std::forward<L>(l), std::forward<F1>(f1), std::forward<F2>(f2), std::forward<Args>(args)...);
         verify_result(
            f1._name + " - " + f2._name,
            ret.first, ret.second,
            end1, end2);
      }
   };


   struct PairItRunner 
   {

      ///
      /// @brief: compare two pairs with results
      ///
      template<
         typename P1, //models pair<I,I>
         typename P2 //models pair<I,I>
      >
      void verify_result(const std::string& msg, const P1& i, const  P2& ii) const
      {
         std::cout << "Comparison for --> " << msg << " -- " << std::boolalpha << (i == ii) << "\n\n";
         assert(&(*i.first) == &(*ii.first));
         assert(&(*i.second) == &(*ii.second));
      }

      ///
      /// @brief: run interface
      ///
      template<
         typename L,         //Launcher
         typename F1,       //stl algo
         typename F2,       //tail recursive algo
         typename... Args> //list of arguments
         void operator()(L&& l, F1&& f1, F2&& f2, Args&&... args) const
      {
         auto ret = _run(std::forward<L>(l), std::forward<F1>(f1), std::forward<F2>(f2), std::forward<Args>(args)...);
         verify_result(f1._name + " - " + f2._name, ret.first, ret.second);
      }
   };

   ///
   /// @brief: models Runner using a policy based design
   ///
   template<typename R>  
   struct Run {

      R r;

      template<
         typename L,         //Launcher
         typename F1,       //stl algo
         typename F2,       //tail recursive algo
         typename... Args> //list of arguments
         void operator()(L&& l, F1&& f1, F2&& f2, Args&&... args) const
      {
         r(std::forward<L>(l), std::forward<F1>(f1), std::forward<F2>(f2), std::forward<Args>(args)...);
      }
   };
}
