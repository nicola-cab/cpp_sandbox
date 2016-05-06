#pragma once

///
/// @brief: check if the tree is a bst
///
#include <limits>
#include <algorithm>
#include <cmath>
#include "rb_tree.h"
#include "rb_tree_node.h"

namespace rb_tree_integrity
{
   ///
   /// @brief: check some properties of the tree passed
   ///
   template<typename T>
   class check_tree
   {
      using Node = rb_tree_node::Node<T>*;
      
   public:
      
      ///
      /// @brief: check if the tree is a bst or not
      /// @param: set - tree to validate
      /// @return: true/false
      ///
      bool is_bst(const tree::set<T>& set);
      
      ///
      /// @brief: check if the tree is balanced or not
      /// @param: set = tree to validate
      /// @return: true/false
      /// @note: the underneath implementation is a red black tree.
      ///        In case of AVL trees to check if the tree is balanced results
      ///        in checking the height of the left subtree against the height of
      ///        the right subtree. This two must not differ more than 1 unit
      ///        In case of red black trees, the follow properties apply:
      ///        - link that goes to right sub tree is never black
      ///        - if the node is not the root there are no 2 consecutives red left links
      ///
      bool is_tree_balanced(const tree::set<T>& set);
      
      ///
      /// @brief: check if the size of the rooted sub-tree is consistent
      /// @param: set - tree to validate
      /// @return: true/false
      ///
      bool is_count_consistent(const tree::set<T>& set);
      
   private:
      
      ///
      /// @brief: private implementation to check if the tree is a bst
      /// @param: node n
      /// @param: min - minimum element in the subtree
      /// @param: max - maximum element in the subtree
      /// @return: true/false
      ///
      bool is_bst(const Node n, const T& min, const T& max);
      
      ///
      /// @brief: check whether the tree is balanced or not
      /// @param: n - root of the tree to analyze
      /// @return: true/false
      ///
      bool is_tree_balanced(const Node root, const Node n);
      
      ///
      /// @brief: private implemementation to check if the count field is consistent
      /// @param: n - root of the tree to examinate
      /// @return: true/false
      ///
      bool is_count_consistent(const Node n);
      
      ///
      /// @brief: return whether the node passed is red or not
      /// @param: n
      /// @return: true/false
      ///
      bool is_red(const Node n);
      
   };
   
   ///
   /// Implementation (public interface)
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
   bool check_tree<T>::is_tree_balanced(const tree::set<T> &set)
   {
      return is_tree_balanced(set.root_, set.root_);
   }

   
   template<typename T>
   bool check_tree<T>::is_count_consistent(const tree::set<T>& set)
   {
      return is_count_consistent(set.root_);
   }
   
   ///
   /// private implementation
   /// (all this algorithms use recursion. Then they are not suitable for huge trees)
   ///
   
   template<typename T>
   bool check_tree<T>::is_bst(const Node n, const T& min, const T& max)
   {
      if( n == nullptr )
         return true;
      
      return
      (n->val_ >= min && n->val_ <= max) &&  //check if the number is the range
      (is_bst(n->left_, min, n->val_)) &&    //recur on left sub-tree
      (is_bst(n->right_, n->val_, max));     //recur on the right sub-tree
   }
   
   template<typename T>
   bool check_tree<T>::is_tree_balanced(const Node root, Node n)
   {
      if( n == nullptr )
         return true;
      
      if( is_red(n->right_) )
         return false;
      
       if (n != root && is_red(n) && is_red(n->left_))
          return false;
      
      return is_tree_balanced(root, n->left_) && is_tree_balanced(root, n->right_);
      
   }
   
   template<typename T>
   bool check_tree<T>::is_red(const Node n)
   {
      if( n != nullptr )
         n->is_red();
      return false;
   }
   
   template<typename T>
   bool check_tree<T>::is_count_consistent(const Node n)
   {
      if( n == nullptr )
         return true;
      
      if(n->count_ != rb_tree_node::size(n->left_) + rb_tree_node::size(n->left_) + 1)
         return false;
      
      return is_count_consistent(n->left_) && is_count_consistent(n->right_);
   }
}