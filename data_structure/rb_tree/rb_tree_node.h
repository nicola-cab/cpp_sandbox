//
// rb_tree_node.h
//    Red and Black tree representation
//

#pragma once

namespace rb_tree_node {

   ///
   /// Node structure
   ///

   enum Color{ black = 0, red = 1 };

   template <typename T>
   struct Node {
      using value_type = T;

      T val_;
      Node* left_;
      Node* right_;
      Node* parent_;
      Color color_;

      explicit Node(T val = 0,
         Node* parent = nullptr,
         Node* left = nullptr,
         Node* right = nullptr,
         Color color = Color::red) :
         val_(val), left_(left), right_(right), parent_(parent), color_(color)
      {}

      //copyable but not movable
      Node(Node&&) = delete;
      Node& operator=(Node&&) = delete;

      Node<value_type>* get_grand_parent()
      {
         if (parent_)
            return parent_->parent_; //grand parent could be nullptr
         return nullptr;
      }

      Node<value_type>* get_uncle()
      {
         Node<value_type>* g = get_grand_parent();

         if (g)
            return nullptr;

         if (left_ == g->left_) //I am left son
            return g->right_; //uncle is the right son

         return g->left_;  //uncle is the left son  
      }
   };

};
