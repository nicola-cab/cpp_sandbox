// RB_Tree.cpp : Defines the entry point for the console application.
//


//to see how much standard library kicks my ass
#include <set>
#include <chrono>
#include <random>
#include <algorithm>
#include <iostream>
#include <string>

#include "rb_tree.h"

using namespace  std;

const unsigned int SIZE = 1000;

template<typename S,typename T>
S insert_set(const std::vector<T>& v)
{
   S set_{};

   auto start = chrono::steady_clock::now();
   
   for (auto& value : v)
   {
      //cout << "Inserting value =" << value << endl;
      set_.insert(value);
   }

   auto end = chrono::steady_clock::now();
   auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);

   std::cout << duration.count() << " ms "<<std::endl;

   return set_;
}

template<typename Result, typename Set, typename Value>
Result find(const std::string& name, Set& set, const Value& value)
{
   auto start = chrono::steady_clock::now();

   Result res = set.find(value);

   auto end = chrono::steady_clock::now();
   auto duration = chrono::duration_cast<chrono::nanoseconds>(end - start);

   
   std::cout << "Tree used -> " << name << " --> time taken = " << duration.count() << " ns" << endl;

   return res;
}

template <typename Set, typename Value>
void erase( Set& s, const Value& v)
{
   //test erase element
   try {
      auto element = s.erase(v);
      cout << "\n\n Deleted node : " << element << endl;
   }
   catch (std::exception & e) {
      cout << e.what() << endl;
   }
}

int main(int argc, char* argv[])
{
   auto N = SIZE;
   if(argc == 2 )
      N = std::atoi(argv[1]);
   
      
   std::default_random_engine engine(std::random_device{}());
   std::uniform_int_distribution<> dist(1, N);

   std::vector<int> myvector;
   for (int i = 1; i < N; ++i)
      myvector.push_back(dist(engine));

   std::cout << "STL set used takes to insert N=" << N << " elements .... ";
   std::set<int> stl_set_ = insert_set<decltype(stl_set_), int>(myvector);

   std::cout << "My implementation set takes to insert N=" << N << " elements .... ";
   tree::set<int> my_set_ = insert_set<decltype(my_set_), int>(myvector);
   
   
   //generate pseudo casual number.
   
   std::default_random_engine engine2(std::random_device{}());
   auto value = dist(engine);

   //find using my set
   try { 
      auto r = find<int, decltype(my_set_), decltype(value)>("rb tree",my_set_,value);
      cout << "Value found using my rb_tree = " << r << endl;
   }
   catch (std::exception & e) {
      cout << e.what() << endl;
   }

   //find using stl
   auto r = find<set<int>::iterator, 
      decltype(stl_set_), 
      decltype(value)> ("stl set",stl_set_, value);

   if (r != stl_set_.end() )
   {
      cout << "Value found using stl set = " << *(r) << endl;
   }
   else
   {
      cout << "Value not found using stl tree!!! "<< endl;
   }

   /*
   try {
      auto res = my_set_.floor(5);
      cout << " floor(5) =" << res << endl;
   }
   catch (std::exception& e) {
      cout << e.what() << endl;
   }

   try {
      auto res = my_set_.ceiling(5);
      cout << " ceiling(5) =" << res << endl;
   }
   catch (std::exception& e) {
      cout << e.what() << endl;
   }*/
   

   //tree traversal
   if ( N <= 10 )
      my_set_.level_walk();
   
   return 0;
}

