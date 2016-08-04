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
      std::size_t count_;  //!<size of the subtree rooted

      explicit Node(T val = 0,
         Node* parent = nullptr,
         Node* left = nullptr,
         Node* right = nullptr,
         Color color = Color::red, 
         std::size_t count = 1) :
         val_(val), left_(left), right_(right), parent_(parent), color_(color), count_(count)
      {}

      //neither copyable nor movable
      Node(const Node&) = delete;
      Node& operator=(const Node&) = delete;
      Node(Node&&) = delete;
      Node& operator=(Node&&) = delete;

     
      ///
      /// @brief: return whether this node is red or black
      /// @return: true/false
      ///
      bool is_red() const
      {
         return color_ == Color::red;
      }
      
   };
   
   ///
   /// @brief: get grand parent for the current node
   /// @return: pointer to the node that is the grand parent of this node
   ///
   template<typename T>
   Node<T>* get_grand_parent(const Node<T>* node)
   {
      if (node->parent_)
         return node->parent_->parent_; //grand parent could be nullptr
      return nullptr;
   }
   
   ///
   /// @brief: get uncle for the current node
   /// @return: point to the node that is the uncle of the this node
   ///
   template<typename T>
   Node<T>* get_uncle(const Node<T>* node)
   {
      auto grand_parent = get_grand_parent(node);
      
      if (grand_parent == nullptr)
         return nullptr;
      
      if (node->parent_ == grand_parent->left_) //I am left leaf
         return grand_parent->right_; //uncle is the right leaf
      
      return grand_parent->left_;  //uncle is the left leaf
   }
   
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
   
   ///
   /// @brief: compute the counter (size of the rooted subtree) from current node to the its parent
   /// @param: node - leaf just inserted in the tree. The algorithm is going to walk through the all the parents
   ///                in order to restore the right counter
   ///
   template<typename T>
   void compute_counter_bottom_up(const Node<T>* node)
   {
      while( node && node->parent_ )
      {
         node->parent_->count_ = size(node->parent_->left_) + size(node->parent_->right_) + 1;
         node = node->parent_;
      }
   }
   
   template<typename T>
   void compute_counter_top_down(Node<T>* node)
   {
      if( node == nullptr )
         return;
      
      compute_counter_top_down(node->left_);
      compute_counter_top_down(node->right_);
      node->count_ = 1 + size(node->left_) + size(node->right_) ;

   }
};
