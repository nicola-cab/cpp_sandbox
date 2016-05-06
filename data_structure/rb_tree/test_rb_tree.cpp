// RB_Tree.cpp : Defines the entry point for the console application.
//

//include "stdafx.h"

#include "rb_tree_test.h"


int main(int argc, char* argv[])
{
   std::set <int> stl_set;
   tree::set<int> tree_set;

   auto key = test_insert(stl_set, tree_set);
   test_swap_equality();
   test_find(key, stl_set, tree_set);
   test_lower_upper_bound(key, stl_set, tree_set);
   //test_erase(key, stl_set, tree_set);
   test_equality_tree(stl_set, tree_set);
   
   //test correctness of the rb_tree
   test_integrity_check(tree_set);

   return 0;
}

