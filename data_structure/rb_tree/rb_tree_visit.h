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
#include <stack>

using rb_tree_node::Node;

namespace rb_tree {

   namespace priv
   {

      ///
      /// private implementation in order visit 
      ///
      template <typename T>
      void _visit_in_order(Node<T> *node, std::vector<T>& ref)
      {
         std::stack<Node<T>*> s;
         while (!s.empty() || node != nullptr)
         {
            if (node != nullptr)
            {
               s.push(node);
               node = node->left_;
            }               
            else 
            {
               auto n = s.top();
               s.pop();
               ref.push_back(n->val_);
               node = n->right_;
            }
         }
      }

      ///
      /// pre order visit 
      ///
      template<typename T>
      void _visit_pre_order(Node<T>* node, std::vector<T>& ref)
      {
         std::stack<Node<T>*> s;
         while (!s.empty() || node != nullptr)
         {
            if (node != nullptr)
            {
               ref.push_back(node->val_);
               if (node->right_ != nullptr)
                  s.push(node->right_);
               node = node->left_;
            }
            else
            {
               node = s.top();
               s.pop();
            }
         }
      }

      ///
      /// visit post order
      ///
      template<typename T>
      void _visit_post_order(Node<T>* node, std::vector<T>& ref)
      {
         std::stack<Node<T>*> s;
         Node<T>* lastVisited = nullptr;
         while (!s.empty() || node != nullptr)
         {
            if (node != nullptr)
            {
               s.push(node);
               node = node->left_;
            }
            else
            {
               auto n = s.top();
               if (n->right_ != nullptr && n->right_ != lastVisited)
               {
                  node = n->right_;
               }                  
               else
               {
                  ref.push_back(n->val_);
                  lastVisited = s.top();
                  s.pop();
               }   
            }
         }
      }

      ///
      /// level visit
      ///
      template <typename T>
      void _level_visit(Node<T>* node, std::vector<T>& ref)
      {
         using Node = Node<T>*;

         if (node == nullptr)
            return;

         std::queue<Node> q_;
         q_.push(node);

         while (!q_.empty()) 
         {
            auto tmp = q_.front();
            ref.push_back(tmp->val_);
            q_.pop();

            if (tmp->left_ != nullptr)
               q_.push(tmp->left_);
            if (tmp->right_ != nullptr)
               q_.push(tmp->right_);

         }
      }
   }

   ///
   /// visit in order
   ///
   template <typename T>
   std::vector<T> visit_in_order(Node<T> *node)
   {
      std::vector<T> ref;
      priv::_visit_in_order(node, ref);
      return ref;
   }

   ///
   /// visit pre order
   ///
   template<typename T>
   std::vector<T> visit_pre_order(Node<T>* node)
   {
      std::vector<T> ref;
      priv::_visit_pre_order(node, ref);
      return ref;
   }

   ///
   /// visit post order
   ///
   template<typename T>
   std::vector<T> visit_post_order(Node<T>* node)
   {
      std::vector<T> ref;
      priv::_visit_post_order(node, ref);
      return ref;
   }

   ///
   /// level visit
   ///
   template <typename T>
   std::vector<T> level_visit(Node<T>* node)
   {
      std::vector<T> ref;
      priv::_level_visit(node, ref);
      return ref;     
   }
};