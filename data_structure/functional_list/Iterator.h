//
//  Iterator.h
//
//  Created by Nicola Cabiddu on 05/06/2016.
//  Copyright © 2016 Nicola Cabiddu. All rights reserved.
//

#ifndef Iterator_h
#define Iterator_h

#include "MyList.h"
#include <stdexcept>


template <typename Collection>
class Iterator
{
   using node_type = typename Collection::Node*;
   using value_type = typename Collection::value_type;
  
public:
   
   explicit Iterator(node_type ptr) :
      ptr_(ptr)
   {}
   
   node_type const operator++()
   {
      //prefix increment
      ptr_ = ptr_->next_.get();
      return ptr_;
   }
   
   node_type const operator++(int)
   {
      //postfix increment
      auto tmp = ptr_;
      operator++();
      return tmp;  
   }
   
   value_type operator*() const
   {
      if( ptr_ )
         return ptr_->val_;
      
      throw std::logic_error("Iterator not dereferencable");
   }
   
   friend bool operator!=(const Iterator<Collection>& x, const Iterator<Collection>& y)
   {
      return x.ptr_ != y.ptr_;
   }
   
   operator bool() const
   {
      return ptr_ != nullptr;
   }
   
   bool is_valid() const
   {
      return ptr_ != nullptr;
   }
   
private:
   
   node_type ptr_;
   
};


#endif /* Iterator_h */
