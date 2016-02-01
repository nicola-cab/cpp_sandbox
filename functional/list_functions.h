#pragma once

#include "List.h"
#include <iostream>

namespace list_function
{
   template<class Beg, class End>
   auto
      fromIterator(Beg it, End end) -> List<typename Beg::value_type>
   {
      typedef typename Beg::value_type T;
      if (it == end) return List<T>();
      return List<T>(*it, fromIterator(std::next(it), end));
   }

   template<class T>
   List<T>
      concat(List<T> a, List<T> b)
   {
      if (a.isEmpty()) return b;
      return List<T>(a.front(), concat(a.popped_front(), b));
   }

   template<class U, class T, class F>
   List<U>
      fmap(F f, List<T> lst)
   {
      if (lst.isEmpty())
         return List<U>();
      else
         return
         List<U>(f(lst.front()), fmap<U>(f, lst.popped_front()));
   }

   template<class T, class P>
   List<T>
      filter(P p, List<T> lst)
   {
      if (lst.isEmpty()) return List<T>();

      if (p(lst.front()))
         return List<T>(lst.front(), filter(p, lst.popped_front()));
      else
         return filter(p, lst.popped_front());
   }

   template<class T, class U, class F>
   U
      foldr(F f, U acc, List<T> lst)
   {
      if (lst.isEmpty())
         return acc;
      else
         return f(lst.front(), foldr(f, acc, lst.popped_front()));
   }

   template<class T, class U, class F>
   U
      foldl(F f, U acc, List<T> lst)
   {
      if (lst.isEmpty())
         return acc;
      else
         return foldl(f, f(acc, lst.front()), lst.popped_front());
   }

   template<class T, class F>
   void forEach(List<T> lst, F f)
   {
      if (!lst.isEmpty()) {
         f(lst.front());
         forEach(lst.popped_front(), f);
      }
   }

   template<class T>
   void
      print(List<T> lst)
   {
      forEach(lst, [](const T& v)
      {
         std::cout << "(" << v << ")";
      });
      std::cout << std::endl;
   }
}

