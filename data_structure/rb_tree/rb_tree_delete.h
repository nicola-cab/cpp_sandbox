///
/// rb_tree_delete.h
///   red black tree erase operations and fixup
///

#pragma once

#include "rb_tree_operation.h"
#include "rb_tree_node.h"

using rb_tree_node::Node;
using rb_tree_node::Color;

namespace rb_tree {

   template<typename T>
   inline void rb_tree_delete_fix_up(Node<T>*& root, Node<T>*& node)
   {
      using Node = Node< T >*;
      Node w = nullptr;

      while ((root != node) && (node != nullptr && node->color_ == Color::black))
      {
         if (node == node->parent_->left_)
         {
            //left sub tree
            w = rb_tree_node::get_uncle(node);

            if (w->color_ == Color::red)
            {
               //sibling is red (change sibling colors and rotate left parent)
               w->color_ = Color::black;
               node->parent_->color_ = Color::red;
               rotate_left(root, node->parent_);
               w = node->parent_->right_;
            }

            if ((w->left_->color_ == Color::black) && (w->right_->color_ == Color::black))
            {
               //sibling with both children black
               w->color_ = Color::red;
               node = node->parent_;
            }
            else if (w->right_->color_ == Color::black)
            {
               //sibling with one child red and the other black
               w->left_->color_ = Color::black;
               w->color_ = Color::red;
               rotate_right(root, w);
               w = node->parent_->right_;
            }

            w->color_ = node->parent_->color_;
            node->parent_->color_ = Color::black;
            w->right_->color_ = Color::black;
            rotate_left(root, node->parent_);
            node = root;
         }
         else //node == node->parent_->right_
         {
            w = rb_tree_node::get_uncle(node);

            if (w->color_ == Color::red)
            {
               //sibling is red (change sibling colors and rotate left parent)
               w->color_ = Color::black;
               node->parent_->color_ = Color::red;
               rotate_right(root, node->parent_);
               w = node->parent_->right_;
            }

            if ((w->left_->color_ == Color::black) && (w->right_->color_ == Color::black))
            {
               //sibling with both children black
               w->color_ = Color::red;
               node = node->parent_;
            }
            else if (w->right_->color_ == Color::black)
            {
               //sibling with one child red and the other black
               w->left_->color_ = Color::black;
               w->color_ = Color::red;
               rotate_left(root, w);
               w = node->parent_->right_;
            }

            w->color_ = node->parent_->color_;
            node->parent_->color_ = Color::black;
            w->right_->color_ = Color::black;
            rotate_right(root, node->parent_);
            node = root;

         }
      }
   }

   ///
   /// Transplat operation
   ///
   template<typename T>
   inline void transplant(Node<T>*& root, Node<T>*u, Node<T>*v)
   {
      if (u->parent_ == nullptr)
         root = v;
      else if (u == u->parent_->left_)
         u->parent_->left_ = v;
      else
         u->parent_->right_ = v;
      if (v != nullptr)
         v->parent_ = u->parent_;
   }

   template <typename T>
   Node<T>* binary_erase(Node<T>*& root, const T& key)
   {

      using Node = Node<T>*;
      Node n = find(root, key);

      //extension for classic binary tree are:
      // variable to keep track of original color for node to delete
      // support variables x and y

      Color original_color = n->color_;
      Node x = nullptr;


      if (n == nullptr)
         return n; //error key not found

      // 3 possible cases can raise here.
      //    A: Delete a leaf (no children)
      //    B: Delete a node with one child
      //    C: Delete a node with two children

      if (n->left_ == nullptr)
      {
         x = n->right_;  //save right sub-tree
         transplant(root, n, n->right_);  //only right child
      }

      else if (n->right_ == nullptr)
      {
         x = n->left_;  //save left sub-tree
         transplant(root, n, n->left_);   //only left child
      }

      else
      {
         //find min tree
         Node y = find_min(n->right_);

         //extension for RB tree
         original_color = y->color_;
         x = y->right_;

         if (y->parent_ == n)
         {
            if (x != nullptr )
               x->parent_ = y;
         }
         else
         {
            transplant(root, y, y->right_);
            y->right_ = n->right_;
            y->right_->parent_ = y;
         }

         transplant(root, n, y);
         y->left_ = n->left_;
         y->left_->parent_ = y;

         //extension for RB tree
         y->color_ = n->color_;

      }

      //fix up for delete
      if (original_color == Color::black)
      {
         rb_tree_delete_fix_up(root, x);
      }
      
      rb_tree_node::compute_counter(n);
      
      return n;
   }
};