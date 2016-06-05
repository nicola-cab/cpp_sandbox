//
//  MyList.h
//  amazon_test_list
//
//  Created by Nicola Cabiddu on 04/06/2016.
//  Copyright © 2016 Nicola Cabiddu. All rights reserved.
//

#ifndef MyList_h
#define MyList_h

#include "Iterator.h"
#include <initializer_list>
#include <memory>

template<typename T> class Iterator;

template<typename T>
class MyList
{
   using value_type = T;
   using iterator_type = Iterator<MyList<value_type>>;
   
public:
   
   ///
   /// iterator class
   ///
   friend iterator_type;

   ///
   /// empty ctor
   ///
   explicit MyList() = default;
   
   ///
   /// ctor for the list.
   /// \param: init list with all the values to store in the list
   ///
   explicit MyList(std::initializer_list<value_type> args);
   
   ///
   /// return the natural order of the collection, returning a new copy
   ///
   MyList<value_type> reverse() const;
   
   ///
   /// filter the current collection and return a new one with elements that matched the
   /// predicate
   ///
   /// \tparam: Ret  - type returned by the predicate
   /// \tparam: Pred - predicate to apply
   /// \return: new collection
   ///
   template<typename Pred>
   MyList<value_type> filter(Pred pred) const;
   
   ///
   /// map current items in the collection into a new collection produced by the evaluation
   /// of the predicate.
   ///
   /// \tparam: Ret - type returned by the predicate
   /// \tparam: Pred - predicate to apply
   /// \return: new collection that matches the predicate passed as argument
   ///
   template<typename R, typename Pred>
   MyList<R> map(Pred pred) const;
   
   
   ///
   /// evaluate the predicate passed as argument and returns the result of the computation
   ///
   /// \tparam: Ret - type returned by the predicate
   /// \tparam: Pred - predicate to apply
   /// \return: new collection that matches the predicate passed as argument
   ///
   template<typename R, typename Pred>
   R foldLeft(Pred pred) const;
   
   
   ///
   /// public interface to walk through the list
   ///
   
   iterator_type begin() const;
   iterator_type end() const;
   
   ///
   /// comparison operators
   ///
   
   friend bool operator==(const MyList<value_type>& x, const MyList<value_type>& y)
   {
      typename MyList<T>::iterator_type it_x = x.begin();
      typename MyList<T>::iterator_type it_y = y.begin();
      
      while( it_x && it_y ) {
         
         if( *it_x != *it_y )
            return false;
         
         ++it_x; ++it_y;
      }
      return it_x.is_valid() == it_y.is_valid();
   }
   
   friend bool operator!=(const MyList<value_type>& x, const MyList<value_type>& y)
   {
      return !(x == y);

   }
   
   ///
   /// Insertion operation
   ///
   
   void push_back(const value_type& t);
   void push_front(const value_type& t);
   
   
private:
   
   ///
   /// singly linked list abstract representation
   ///
   struct Node {
      
      value_type val_;
      std::unique_ptr<Node> next_;
      
      Node( value_type val ) :
         val_(std::move(val)),
         next_(nullptr)
      {}
      
   };
   
   std::unique_ptr<Node> head_ = nullptr; /*!< head singly linked list */
   Node* tail_ = nullptr;  /*!< pointer to the tail */
   
};


///
/// Implementation
///

template<typename T>
MyList<T>::MyList(std::initializer_list<T> args)
{
   for(const auto& arg : args)
      push_back(arg);
}

template<typename T>
MyList<T> MyList<T>::reverse() const
{
   iterator_type it = begin();
   MyList<T> l;
   while( it ) {
      l.push_front(*it);
      ++it;
   }
   return l;
}

template<typename T>
template<typename Pred>
MyList<T> MyList<T>::filter(Pred pred) const
{
   iterator_type it = begin();
   MyList<T> l;
   while( it ) {
      if( pred(*it) )
         l.push_back(*it);
      ++it;
   }
   return l;
}

template<typename T>
template<typename R, typename Pred>
MyList<R> MyList<T>::map(Pred pred) const
{
   iterator_type it = begin();
   MyList<R> l;
   while( it ) {
      l.push_back(pred(*it));
      ++it;
   }
   return l;
}

template<typename T>
template<typename R, typename Pred>
R MyList<T>::foldLeft(Pred pred) const
{
   iterator_type it = begin();
   R res{};
   while(it) {
      res += pred(*it);
      ++it;
   }
   return res;
}

///
/// Interface to iterate through the list
///

template<typename T>
typename MyList<T>::iterator_type MyList<T>::begin() const
{
   return MyList<T>::iterator_type{head_.get()};
}

template<typename T>
typename MyList<T>::iterator_type MyList<T>::end() const
{
   return MyList<T>::iterator_type{nullptr};
}

///
/// Insert operations
///

template<typename T>
void MyList<T>::push_back(const T& t)
{
   auto node = std::make_unique<Node>(t);
   
   if( head_ == nullptr ) {
      head_ = std::move(node);
      tail_ = head_.get();
   }
   else
   {
      Node* const new_tail = node.get();
      tail_->next_ = std::move(node);
      tail_ = new_tail;
   }
}

template<typename T>
void MyList<T>::push_front(const T& t)
{
   auto node = std::make_unique<Node>(t);

   if( head_ == nullptr ) {
      head_ = std::move(node);
      tail_ = head_.get();
   }
   else {
      Node* const new_head = node.get();
      new_head->next_ = std::move(head_);
      head_ = std::move(node);
   }
}

#endif /* MyList_h */
