#pragma once

///
/// @brief: check if the tree is a bst
///
#include <limits>

namespace rb_tree_integrity
{
   ///
   /// typedef around the type node
   ///
   template<typename T>
   using Node = rb_tree_node::Node<T>;
   
   ///
   /// @brief: check some properties of the tree passed
   ///
   template<typename T>
   class check_tree
   {
            
   public:
      
      ///
      /// @brief: check if the tree is a bst or not
      ///
      bool is_bst(const tree::set<T>& set);
      
      ///
      /// @brief: check if the size of the rooted sub-tree is consistent
      ///
      bool is_count_consistent(const tree::set<T>& set);
      
   private:
      
      bool is_bst(Node<T>* n, const T& min, const T& max);
      
      bool is_count_consistent(Node<T>* n);

   };
   
   ///
   /// Implementation
   ///
   
   template<typename T>
   bool check_tree<T>::is_bst(const tree::set<T> & set)
   {
      //what done here is not really portable.
      //in case of keys that are not scalar types but for which
      //is defined a min and a max this code won't compile
      return is_bst(set.root_, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
   }
   
   template<typename T>
   bool check_tree<T>::is_count_consistent(const tree::set<T>& set)
   {
      return is_count_consistent(set.root_);
   }
   
   template<typename T>
   bool check_tree<T>::is_bst(Node<T>* n, const T& min, const T& max)
   {
      if( n == nullptr )
         return true;
      
      return false;
   }
   
   template<typename T>
   bool check_tree<T>::is_count_consistent(Node<T>* n)
   {
      return false;
   }

   
}