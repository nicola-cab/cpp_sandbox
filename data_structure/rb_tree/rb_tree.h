#pragma once

#include "rb_tree_insert.h"
#include "rb_tree_delete.h"
#include "rb_tree_visit.h"

namespace tree 
{
   class NodeNotFound{};

   template<typename T>
   class set {

      using value_type = T;
      using Node = Node < value_type >;
      Node* root_;

      ///
      /// surprisingly this recursive function 
      /// outperforms a iteratively one (using a queue). 
      ///
      void free_tree(Node* node)
      {
         if (node == nullptr)
            return;
         if (node->left_ != nullptr)
            free_tree(node->left_);
         if (node->right_ != nullptr)
            free_tree(node->right_);
         delete node;
      }
     
      public:

         ///
         /// ctor
         ///
         explicit set() : root_(nullptr) 
         {}

         //
         // copy forbidden 
         //  (it is costly and complicated to implement)
         //
         set(const set& s) = delete; 
         set& operator=(const set& s) = delete; 

         ///
         /// move semantics
         ///
         set(set&& s)
         { 
            root_ = s.root_;
            s.root_ = nullptr;
         }

         ///
         /// dtor for my set
         ///
         ~set()
         {
            if (root_ != nullptr)
            {
               free_tree( root_ );
               root_ = nullptr;  
            }
            
         }

         ///
         /// insertion in rb tree.
         /// @value: value to insert inside rb tree
         ///
         void insert(value_type value) {
            rb_tree::binary_insertion(root_, value);
         }

         ///
         /// erase element from rb tree
         /// @value: value to search and delete from tree
         /// @return: value deleted or throw exception
         ///
         value_type erase(const value_type& value) {
            auto tmp = rb_tree::binary_erase(root_, value);  
            if (tmp != nullptr) {
               // potentional heavy operation here.
               value_type val_ = tmp->val_ ;
               delete tmp;
               return val_; //here compiler should help me .. I guess a copy elison
            }
            else {
               throw std::logic_error("Value to delete not found!");
            }
         }

         ///
         /// find a element inside rb tree
         /// @value: value to find
         /// @return: copy of element just found or throw exception if value is not found
         ///
         value_type find(const value_type& value) const
         {
            auto node = rb_tree::find(root_, value);
            if (node == nullptr)
               throw std::logic_error("Value not found!");
            return node->val_;
         }

         ///
         /// walk through the tree using a visit in order
         ///
         void tree_walk() const
         {
            rb_tree::visit_in_order(root_);
         }

         ///
         /// level walk traversal for tree
         ///
         void level_walk()
         {
            rb_tree::level_visit(root_);
         }

         /// utilities operation
         bool is_empty() const
         {
            return root_ == nullptr;
         }

         /// order statist operation for tree

         ///
         /// min function
         /// @return: the minimun element or throw exception if container is empty
         ///
         value_type min() const
         {
            if (!is_empty())
            {
               auto res_ = rb_tree::find_min(root_);
               return res_->val_;
            }
               

            throw std::logic_error("empty set");
         }
         ///
         /// max function
         /// @return: the maximum element or throw exception if container is empty 
         ///
         value_type max() const
         {
            if ( !is_empty() )
            {
               auto res_ = rb_tree::find_max(root_);      
               return res_->val_;
            }
            throw std::logic_error("empty set");
         }

         ///
         /// floor function: 
         /// @desc: the function returns the largest previous element in the tree for key.
         ///         floor(x) = |_ x _|
         /// @return: the floor of x or a logic error if invoked on empty set 
         ///
         value_type floor( const value_type& x ) const 
         {
            if (!is_empty())
            {
               auto res_ = rb_tree::floor(root_, x);
               if ( res_ != nullptr )
                  return res_->val_;
            }
            throw std::logic_error("value not found");
         }
         
         ///
         /// ceiling function: 
         /// @desc: the function returns the largest previous element in the tree for key.
         ///         ceiling(x) = [x]
         /// @return: the ceiling of x or a logic error if invoked on empty set 
         ///
         value_type ceiling(const value_type& x) const
         {
            if (!is_empty())
            {
               auto res_ = rb_tree::ceiling(root_, x);
               if ( res_ != nullptr)
                  return res_->val_;
            }
            throw std::logic_error("value not found");
         }

   };

}