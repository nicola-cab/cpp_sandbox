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
      /// @brief: check if the tree is 23
      /// @param: set = tree to validate
      /// @return: true/false
      /// @note: the underneath implementation is a red black tree.
      ///        In case of AVL trees to check if the tree is balanced I would have to
      ///        check the height of the left subtree against the height of
      ///        the right subtree. This two must not differ more than 1 unit
      ///        In case of red black trees, the follow properties apply:
      ///        - link that goes toward right sub tree is never black
      ///        - if the node is not the root there are no 2 consecutives left links that are red
      ///        - essentially I am checking if the tree is a 2-3 tree
      ///
      bool is_tree_23(const tree::set<T>& set);
      
      ///
      /// @brief: check if the tree is balanced
      /// @param: set
      /// @return: true/false
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
      ///         (actually this method checks if the tree is a 23 tree)
      /// @param: n - root of the tree to analyze
      /// @return: true/false
      ///
      bool is_tree_23(const Node root, const Node n);
      
      ///
      /// @brief: check whether every path from the root to the a leaf
      ///         has the same number of black links
      ///
      bool is_tree_balanced(const Node root, const Node parent, const Node node, int blacks);
      
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
   bool check_tree<T>::is_tree_23(const tree::set<T> &set)
   {
      return is_tree_23(set.root_, set.root_);
   }
   
   template<typename T>
   bool check_tree<T>::is_tree_balanced(const tree::set<T>& set)
   {
      //number of black nodes
      int black = 0;
      Node x = set.root_;
      while (x != nullptr)
      {
         if (!is_red(x) ) //&& x!=set.root_)
            black++;
         
         x = x->left_;
      }
      return is_tree_balanced(set.root_, set.root_, set.root_, black);
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
   bool check_tree<T>::is_tree_23(const Node root, Node n)
   {
      if( n == nullptr )
         return true;
      
      //if node is red then both its children must be black
      if( is_red(n) && (is_red(n->left_) || is_red(n->right_)) )
         return false;
      
       //there are no 2 consecutives links that are red that go to the left
       if (n != root && is_red(n) && is_red(n->left_))
          return false;
      
      return is_tree_23(root, n->left_) && is_tree_23(root, n->right_);
      
   }
   
   template<typename T>
   bool check_tree<T>::is_tree_balanced(const Node root, const Node parent, const Node node, int blacks)
   {
      if( node == nullptr )
      {
         //I am actually tracking down the links (not the nodes)
         if(root == parent && blacks > 0)
            --blacks;
         return (blacks == 0);
      }
      
      if( !is_red(node)) --blacks;
      
      return is_tree_balanced(root, node, node->left_, blacks) && is_tree_balanced(root, node, node->right_, blacks);
   }
   
   template<typename T>
   bool check_tree<T>::is_red(const Node n)
   {
      if( n != nullptr )
         return n->is_red();
      
      return false;
   }
   
   template<typename T>
   bool check_tree<T>::is_count_consistent(const Node n)
   {
      if( n == nullptr )
         return true;
      
      if(n->count_ != rb_tree_node::size(n->left_) + rb_tree_node::size(n->right_) + 1)
         return false;
      
      return is_count_consistent(n->left_) && is_count_consistent(n->right_);
   }
}