//
// rb_tree_node.h
//    Red and Black tree representation
//

#pragma once

#include <cstddef>

namespace rb_tree_node 
{

   ///
   /// Node structure
   ///

   enum Color{ black = 0, red = 1 };

   template <typename T>
   struct Node 
   {
      using value_type = T;

      T val_;              //!< value held in the node  
      Node* left_;         //!< left sub tree 
      Node* right_;        //!< right sub tree 
      Node* parent_;       //!<pointer to the parent 
      Color color_;        //!<color of the node (balacing) 
      std::size_t count_;   //!<size of the subtree rooted

      explicit Node(T val = 0,
         Node* parent = nullptr,
         Node* left = nullptr,
         Node* right = nullptr,
         Color color = Color::red, 
         std::size_t count = 0) :
         val_(val), left_(left), right_(right), parent_(parent), color_(color), count_(count)
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

         if (left_ == g->left_) //I am left leaf
            return g->right_; //uncle is the right leaf

         return g->left_;  //uncle is the left leaf  
      }
   };

   ///
   /// @brief: return the size of the rooted subtree
   /// 
   template <typename T>
   std::size_t size(const Node<T>* node)
   {
      if (node)
         return node->count_;
      return 0;
   }
};
