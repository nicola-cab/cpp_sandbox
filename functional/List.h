#pragma once

#include <memory>
#include <cassert>
#include <initializer_list>

template<typename T>
class List
{
   struct Item
   {
      Item(T v, std::shared_ptr<const Item> const & tail)
         : _val(v), _next(tail) {}

      T _val;
      std::shared_ptr<const Item> _next;
   };

   explicit List(std::shared_ptr<const Item> const & items) :
      _head(items) {}

   std::shared_ptr<const Item> _head;


public:

   explicit List() : _head() 
   {}

   List(T v, List tail)
      : _head(std::make_shared<Item>(v, tail._head)) 
   {}

   List(std::initializer_list<T> init) : _head()
   {
      for (auto& val : init)
      {
         _head = std::make_shared(val, _head);
      }
   }

   bool isEmpty() const 
   { return !_head; }

   T front() const
   {
      assert(!isEmpty());
      return _head->_val;
   }

   List popped_front() const
   {
      assert(!isEmpty());
      return List(_head->_next);
   }

   List pushed_front(T v) const
   {
      return List(v, *this);
   }

   List insertedAt(int i, T v) const
   {
      if (i == 0)
         return pushed_front(v);
      else {
         assert(!isEmpty());
         return
            List(front(),
            popped_front().insertedAt(i - 1, v));
      }
   }
};

