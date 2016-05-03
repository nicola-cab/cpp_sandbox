// RB_Tree.cpp : Defines the entry point for the console application.
//

//include "stdafx.h"

#include "rb_tree.h"

#include <set>
#include <chrono>
#include <random>
#include <algorithm>
#include <iostream>
#include <chrono>
#include <unordered_set>

using namespace  std;

const unsigned int N = 10;

using Clock = chrono::steady_clock;

///
/// util functions
///

template<typename T>
void random_vector(std::vector<T>& v, T& ret_val)
{
   std::default_random_engine engine(std::random_device{}());
   std::uniform_int_distribution<> dist(0, N - 1);
   auto randomIndex = dist(engine);

   v.reserve(N);
   for (int i = 0; i < N; ++i)
   {
      auto elem = dist(engine);
      //store a casual number that will belong to the array
      if (i == randomIndex)
         ret_val = elem;
      v.push_back(elem);
   }
}

template<typename T>
void asceding_vector(std::vector<T>& v, T& ret_val)
{
   std::default_random_engine engine(std::random_device{}());
   std::uniform_int_distribution<> dist(0, N - 1);
   auto randomIndex = dist(engine);

   v.reserve(N);
   for (int i = 0; i < N; ++i)
   { 
      //store a casual number that will belong to the array
      if (i == randomIndex)
         ret_val = i;
      v.push_back(i);
   }
}


template<typename T>
void desceding_vector(std::vector<T>& v, T& ret_val)
{
   std::default_random_engine engine(std::random_device{}());
   std::uniform_int_distribution<> dist(0, N - 1);
   auto randomIndex = dist(engine);

   v.reserve(N);
   for (int i = N-1; i >= 0; --i)
   {
      //store a casual number that will belong to the array
      if (i == randomIndex)
         ret_val = i;
      v.push_back(i);
   }
}


template<typename Set, typename T>
void insert_set(Set &set, const std::vector<T>& v)
{
   auto start = Clock::now();

   for (auto& value : v)
      set.insert(value);

   auto end = Clock::now();
   auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);

   std::cout << duration.count() << " ms " << std::endl;
}

template<typename Result, typename Set, typename Value>
Result find(Set& set, const Value& value)
{
   auto start = Clock::now();

   Result res = set.find(value);

   auto end = Clock::now();
   auto duration = chrono::duration_cast<chrono::nanoseconds>(end - start);


   std::cout << "Find element has taken = " << duration.count() << " ns" << endl;

   return res;
}

template <typename Set, typename T>
void erase(Set& s, const T& v)
{
   auto start = Clock::now();

   s.erase(v);

   auto end = Clock::now();
   auto duration = chrono::duration_cast<chrono::nanoseconds>(end - start);

   std::cout << "Delete a element has taken = " << duration.count() << " ns" << endl;
}

///
/// tests
///
template<typename T>
void test_equality_tree(std::set<T>& stl_set, tree::set<T>& tree_set)
{
   std::cout << "\nCompare trees \n";
   std::vector<T> vv{ stl_set.begin(), stl_set.end() }; 
   const auto v = tree_set.in_order_walk(); 
   assert(vv == v);
   std::cout << std::boolalpha << "trees are equal = "<< (vv == v) << std::endl;

   /*for (auto ii = v.begin(); ii != v.end(); ++ii)
      std::cout << *ii << " ";
   std::cout << std::endl;*/
}

template<typename T>
T test_insert(std::set<T>& stl_set, tree::set<T>& tree_set)
{
  
   std::vector<T> v;
   T ret;

   //random_vector(v, ret);
   //asceding_vector(v, ret);
   //desceding_vector(v, ret);

   //simple vector 
   tree::set<T> tree_set2;
	std::vector<T> v2 {5,3,2,1,6};
   v.swap(v2);
   ret = 3;
      
   std::cout << "STL set has taken to insert N=" << N << " elements .... \n";
   insert_set(stl_set, v);
   
   std::cout << "Crafted RB Tree algorithm has taken N=" << N << " elements .... \n";
   insert_set(tree_set, v);
   insert_set(tree_set2, v);
   tree_set2.swap(tree_set);
   

   return ret;
}

template<typename T>
void test_find(const T& key, const std::set<T>& stl_set, const tree::set<T>& tree_set)
{
   bool found = true;
   T val; 

   //find using my set
   try
   {
      cout << "\n--- Find crafted tree --- \n";
      val = find<T>(tree_set, key);
      cout << "Value found using crafted rb tree!!! r= " << val << endl;
   }
   catch (std::exception & e)
   {
      found = false;
      cout << e.what() << endl;
   }

   //find using stl
   cout << "\n--- Find stl tree --- \n";
   auto r = find<typename std::set<T>::const_iterator>(stl_set, key);

   if (r != stl_set.end())
   {
      cout << "Value found using stl set!!! r= " << *(r) << endl;
      assert(*r == val);
   }   
   else
   {
      cout << "Value not found using stl tree!!! " << endl;
   }
}

template<typename T>
void test_lower_upper_bound(const T& key, const std::set<T>& stl_set, const tree::set<T>& tree_set )
{
   T lower = -1, upper = -1;

   // my tree
   cout << "\n--- Lower/Upper bound crafted tree --- \n";
   try 
   {
      lower = tree_set.lower_bound(key);
      cout << " crafted tree ==> lower bound of value " << key << " is " << lower << endl;
   }
   catch (std::exception& e) {
      cout << e.what() << endl;
   }
   try 
   {
      upper = tree_set.upper_bound(key);
      cout << " crafted tree ==> upper bound of value " << key << " is " << upper << endl;
   }
   catch (std::exception& e) 
   {
      cout << e.what() << endl;
   }

   // stl tree
   cout << "\n--- Lower/Upper bound stl tree --- \n";

   auto lit = stl_set.lower_bound(key);
   auto uit = stl_set.upper_bound(key);

   if (lit != stl_set.end() && uit != stl_set.end())
   {
      cout << " stl set ==> lower bound of value " << key << " is " << *lit << endl;
      cout << " stl set ==> upper bound of value " << key << " is " << *uit << endl;
      assert(lower == *lit);
      assert(upper == *uit);
   }
 
}

template<typename T>
void test_erase(const T& key, std::set<T>& stl_set, tree::set<T>& tree_set)
{
   assert(stl_set.size() == tree_set.size());

   //my tree
   cout << "\nElement to erase = " << key << std::endl;

   cout << "--- Erase element crafted tree --- \n";
   try {
     erase(tree_set, key);
   }
   catch (const std::exception& e)
   {
      e.what();
   }

   cout << "--- Erase element stl tree --- \n";
   erase(stl_set, key);

   bool found_tree = false;
   bool found_stl = false;
   try {
      tree_set.find(key);
      found_tree = true;         
   }
   catch (std::exception & e) {
      e.what();
   }
   
   auto it = stl_set.find(key);
   if (it != stl_set.end())
      found_stl = true;

   assert(found_stl == found_tree);
   assert(stl_set.size() == tree_set.size());
}


int main(int argc, char* argv[])
{
   std::set <int> stl_set;
   tree::set<int> tree_set;

   auto key = test_insert(stl_set, tree_set);
   test_find(key, stl_set, tree_set);
   test_lower_upper_bound(key, stl_set, tree_set);
   test_erase(key, stl_set, tree_set);
   test_equality_tree(stl_set, tree_set);

   return 0;
}

