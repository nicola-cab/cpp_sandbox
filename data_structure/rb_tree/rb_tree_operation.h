#pragma once

#include "rb_tree_node.h"
using rb_tree_node::Node;

namespace rb_tree {


   ///
   /// rotate left
   ///
   template < typename T >
   void rotate_left(Node<T>*& root, Node<T>*& x)
   {
      using Node = Node < T >*;

      Node y = x->right_;
      x->right_ = y->left_;

      if (y->left_)
         y->left_->parent_ = x;

      y->parent_ = x->parent_;

      if (x->parent_ == nullptr)
         root = y;

      else if (x == x->parent_->left_)
         x->parent_->left_ = y;

      else
         x->parent_->right_ = y;

      y->left_ = x;
      x->parent_ = y;
   }

   ///
   /// rotate right
   ///
   template<typename T>
   void rotate_right(Node<T>*& root, Node<T>*& y)
   {
      using Node = Node < T >*;

      Node x = y->left_;
      y->left_ = x->right_;

      if (y->left_)
         y->left_->parent_ = y;

      x->parent_ = y->parent_;

      if (y->parent_ == nullptr)
         root = x;

      else if (y == y->parent_->left_)
         y->parent_->left_ = x;

      else
         y->parent_->right_ = x;

      x->right_ = y;
      y->parent_ = x;
   }

   ///
   /// find value in tree
   ///
   template<typename T>
   Node<T>* find(Node<T>* root, const T& key)
   {
      using Node = Node<T>*;

      //search node that contains the key
      Node n = root;
      while ((n != nullptr) && (n->val_ != key))
      {
         if (key < n->val_)
            n = n->left_;
         else
            n = n->right_;
      }

      return n;
   }

   ///
   /// find min value in tree
   ///
   template<typename T>
   Node<T>* find_min(Node<T>* node)
   {
      if (node == nullptr)
         return nullptr;

      while (node->left_ != nullptr)
         node = node->left_;
      return node;
   }

   ///
   /// find max value in tree
   ///
   template<typename T>
   Node<T>* find_max(Node<T>* node)
   {
      if (node == nullptr)
         return nullptr;

      while (node->right_ != nullptr)
         node = node->right_;
      return node;
   }


   /*
   // Floor recursive implementation

   if (node == nullptr)
   return nullptr;

   if (node->val_ == x)
   return node;

   if ( x < node->val_)
   return floor(node->left_, x);

   auto tmp = floor(node->right_, x);
   if (tmp != nullptr)
   return tmp;

   return node;
   */

   ///
   /// floor for Binary Search tree
   /// @desc: floor is the greatest key inside the tree <= x
   /// @param: node (root of tree or subtree in which search)
   /// @param: x    (key to search)
   /// @return: floor key or nullptr
   ///
   template<typename T>
   Node<T>* floor(Node<T>* node, const T& x)
   {
      using Node = Node < T >* ;
      Node floor_ = nullptr;
      while (node != nullptr) {
         if (x < node->val_)
            node = node->left_;
         else {
            floor_ = node;
            node = node->right_;
         }
      }
      return floor_; 
   }

   
   /*
   //ceiling recurisive implementation

   if (node == nullptr)
   return nullptr;

   if (node->val_ == x)
   return node;

   if (x > node->val_)
   return ceiling(node->right_, x);

   auto tmp = ceiling(node->left_, x);
   if (tmp != nullptr)
   return tmp;

   return node;
   */

   ///
   /// ceiling for Binary Search tree:
   /// @desc: ceiling is the lower key inside the tree >= x 
   /// @param: node (root of tree or subtree in which search)
   /// @param: x    (key to search)
   /// @return: ceiling key or nullptr
   ///
   template<typename T>
   Node<T>* ceiling(Node<T>* node, const T& x)
   {
      using Node = Node < T >*;
      Node ceiling_ = nullptr;
      while (node != nullptr) {
         if (x > node->val_)
            node = node->right_;
         else {
            ceiling_ = node;
            node = node->left_;
         }
      }
      return ceiling_;

      
   }

   ///
   /// select the ith element inside tree (i-th order statics)
   /// @param: node
   /// @param: x (th statics order to retrieve)
   /// @return: nullptr or x-th order statistic
   ///
   template <typename T>
   Node<T>* select(Node<T>* node, const T& x)
   {
      return nullptr;
   }

   ///
   /// select the ith element inside tree (i-th order statics)
   /// @param: node
   /// @param: x 
   /// @return: number of keys less than x
   ///
   template <typename T>
   int rank(Node<T>* node, const T& x)
   {
      return 0;
   }
}