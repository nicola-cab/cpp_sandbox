#pragma once

#include "rb_tree_insert.h"
#include "rb_tree_delete.h"
#include "rb_tree_visit.h"

#if defined (WIN32)
#define NOEXCPT _NOEXCEPT
#else
#define NOEXCPT noexcept 
#endif


namespace rb_tree_integrity
{
   template <typename T> class check_tree;
}

namespace tree 
{
   template<typename T>
   class set 
   {
      using value_type = T;
      using Node = Node < value_type >;
      Node* root_;
      std::size_t size_;

      ///
      /// @brief: integrity checks are not part of this class.
      ///         the integrity check algorithms are based on the use of
      ///         the root node for this tree.
      ///         Then I need to make the check_tree class friend of this
      ///
      friend rb_tree_integrity::check_tree<T>;
      
      ///
      /// @brief: erase all the nodes of the tree 
      ///         (no recursion, over 100000 items is not doable)
      ///
      void free_tree(Node* node)
      {
         std::queue<Node*> q;
         q.push(node);

         while (!q.empty())
         { 
            auto n = q.front();
            q.pop();
            
            if (n->left_)
               q.push(n->left_);
            if (n->right_)
               q.push(n->right_);    

            delete n;
         }
      }
     
      public:

         ///
         /// ctor
         ///
         explicit set() : root_(nullptr), size_(0)
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
            size_ = s.size_;
            s.root_ = nullptr;
            s.size_ = 0;
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
               size_ = 0;
            }
            
         }

         ///
         /// insertion in rb tree.
         /// @value: value to insert inside rb tree
         ///
         void insert(value_type value)
         {
            auto res = rb_tree::binary_insertion(root_, value);
            size_ += res == true ? 1 : 0;
         }

         ///
         /// erase element from rb tree
         /// @value: value to search and delete from tree
         /// @return: value deleted or throw exception
         ///
         void erase(const value_type& value) {
            auto tmp = rb_tree::binary_erase(root_, value);  
            if (tmp != nullptr) 
            {
               delete tmp; 
               --size_;
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
         /// in order visit of the tree
         ///
         std::vector<value_type> in_order_walk() const
         {
            return rb_tree::visit_in_order(root_);
         }

         ///
         /// pre order visit of the tree
         ///
         std::vector<value_type> pre_order_walk() const
         {
            return rb_tree::visit_pre_order(root_);
         }

         ///
         /// post order visit of the tree
         ///
         std::vector<value_type> post_order_walk() const
         {
            return rb_tree::visit_post_order(root_);
         }

         ///
         /// level order visit (BSF)
         ///
         std::vector<value_type> level_walk() const
         {
            return rb_tree::level_visit(root_);
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
         value_type lower_bound( const value_type& x ) const 
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
         value_type upper_bound(const value_type& x) const
         {
            if (!is_empty())
            {
               auto res_ = rb_tree::ceil(root_, x);
               if ( res_ != nullptr)
                  return res_->val_;
            }
            throw std::logic_error("value not found");
         }

         ///
         /// @brief: return if 2 trees are equal
         /// @note: is this the best way to perfrom this task? this algorithm takes 
         ///        O(2n) + O(n) to compare plus O(2N) space.
         //         Another algorithm that allows me to stop
         ///        immediately when the first item is missing would be O(n lg n).
         ///        Otherwise a recursive implementation could also work.. even though
         ///        the depth of recursion for certain trees could blow the stack
         ///
         friend inline bool operator == (const set<T>& lhs, const set<T>& rhs) 
         {
            const auto& left = rb_tree::visit_in_order(lhs.root_);
            const auto& right = rb_tree::visit_in_order(rhs.root_);
            return (left == right);
         }

         ///
         /// @brief: return if 2 trees are not equal
         ///
         friend inline bool operator != (const set<T>& lhs, const set<T>& rhs)
         {
            return !(lhs == rhs);
         }

         ///
         /// @brief: return the number of element held 
         ///

         std::size_t size() const
         {
            return size_;
         }

         std::size_t size() 
         {
            return size_;
         }

         /// 
         /// @brief: return weather the tree is empty or not
         ///
         bool empty() const
         {
            return size_ == 0;
         }

         bool empty()
         {
            return size_ == 0;
         }

         ///
         /// @brief: swap 2 trees
         ///
         void swap(set<T>& s) NOEXCPT
         {
            std::swap(s.root_, root_);
         }
   };

}
