//
// rb_tree_visit.h
//    Utilities to walk through a tree: 
//       in_order_walk
//       pre_order_walk
//       post_order_walk
//       level_walk (using queue)
//

#pragma once

#include "rb_tree_node.h"
#include <queue>
#include <iostream>

using rb_tree_node::Node;

namespace rb_tree {

   ///
   /// visit in order
   ///
   template <typename T>
   void visit_in_order(Node<T> *node)
   {
      if (node == nullptr)
         return;

      visit_in_order<T>(node->left_);
      std::cout << node->val_ << std::endl;
      visit_in_order<T>(node->right_);
   }

   ///
   /// visit pre order
   ///
   template<typename T>
   void visit_pre_order(Node<T>* node)
   {
      if (node == nullptr)
         return;

      std::cout << node->val_ << std::endl;
      visit_pre_order<T>(node->left_);
      visit_pre_order<T>(node->right_);
   }

   ///
   /// visit post order
   ///
   template<typename T>
   void visit_post_order(Node<T>* node)
   {
      if (node == nullptr)
         return;

      visit_pvisit_post_orderre_order<T>(node->left_);
      visit_post_order<T>(node->right_);
      std::cout << node->val_ << std::endl;
   }

   ///
   /// level visit
   ///
   template <typename T>
   inline void level_visit(Node<T>* root)
   {
      using Node = Node<T>*;

      if (root == nullptr)
         return;

      std::queue<Node> q_;
      q_.push(root);

      while (!q_.empty()) {
         auto tmp = q_.front();
         std::cout << " - Value: " << tmp->val_ << endl;
         q_.pop();

         if (tmp->left_ != nullptr)
            q_.push(tmp->left_);
         if (tmp->right_ != nullptr)
            q_.push(tmp->right_);

      }
   }
};