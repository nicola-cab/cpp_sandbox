///
/// rb_tree_insert.h
///   red black tree insertion
///

#pragma once

#include "rb_tree_node.h"
#include "rb_tree_operation.h"
#include <cassert>

using rb_tree_node::Node;
using rb_tree_node::Color;

namespace rb_tree {

   // fix-up for rb tree. Fix up made by composition
   // Accordingly with MIT's book "Introduction to alogorithms" - CLRS,
   // there are 6 possible case where it is needed to fix the tree.

   template<typename T>
   void rb_tree_insert_fix_up(Node<T>*& root, Node<T>*& node)
   {
      using Node = Node < T >*;

      Node parent = nullptr;
      Node grand_parent = nullptr;

      while ((node != root) &&
         (node->color_ != Color::black) &&
         (node->parent_->color_ == Color::red))
      {
         parent = node->parent_;
         grand_parent = node->get_grand_parent();

         //Case A:
         //    Parent is left child
         if (parent == grand_parent->left_)
         {
            Node uncle = node->get_uncle();

            // case 1: 
            //    Uncle is red --> recoloring
            if ((uncle != nullptr) && (uncle->color_ == Color::red)) {
               grand_parent->color_ = Color::red;
               parent->color_ = Color::black;
               uncle->color_ = Color::black;
               node = grand_parent;
            }
            else {
               //case 2: 
               //   node is right child.. needed left rotation
               if (node == parent->right_) {
                  node = parent;
                  rotate_left(root, parent);
               }

               //case 3:
               // node is left child... needed right rotate
               parent->color_ = Color::black;
               grand_parent->color_ = Color::red;
               rotate_right(root, grand_parent);
            }

         }
         // Case B:
         //    Parent is right child    
         else
         {
            Node uncle = node->get_uncle();

            //case 1:
            //    Uncle is red
            if (uncle != nullptr && uncle->color_ == Color::red) {
               grand_parent->color_ = Color::red;
               parent->color_ = Color::black;
               uncle->color_ = Color::black;
               node = grand_parent;
            }
            else {
               //case 2: 
               //   node is left child
               if (node == parent->left_)
               {
                  node = parent;
                  rotate_right(root, parent);
               }

               //case 3:
               //    node is right child
               parent->color_ = Color::black;
               grand_parent->color_ = Color::red;
               rotate_left(root, grand_parent);
            }
         }
      }
      //root always black
      root->color_ = Color::black;
   }


   ///
   /// Binary insert
   ///
   template<typename T>
   void binary_insertion(Node<T>*& node, T val)
   {
      Node<T>* z = new Node<T>(val);

      if (node == nullptr)
      {
         node = z;
      }
      else
      {
         Node<T>* parent = nullptr;
         Node<T>* tmp = node;
         while (tmp)
         {
            parent = tmp;
            
            if (val < tmp->val_) {
               tmp = tmp->left_;
            }
            else if( val > tmp->val_) {
               tmp = tmp->right_;
            }
            else {
               tmp->val_ = val;
               break;
            }
         }
         
         assert(parent != nullptr);

         if (val < parent->val_)
         {
            parent->left_ = z;
         }
         else if( val > parent->val_)
         {
            parent->right_ = z;
         }
         //else .. nothing to do

         z->parent_ = parent;
      }

      rb_tree_insert_fix_up(node, z);
   }

};


