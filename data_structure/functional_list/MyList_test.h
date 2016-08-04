//
//  MyList_test.h
//
//  Created by Nicola Cabiddu on 04/06/2016.
//  Copyright © 2016 Nicola Cabiddu. All rights reserved.
//

#ifndef MyList_test_h
#define MyList_test_h

#include "MyList.h"
#include <iostream>
#include <cassert>
#include <vector>
#include <ostream>
#include <algorithm>
#include <string>

///
/// declares a generic test
///
class Test
{
   std::string name_;
   bool value_ = false;
   
public:
   
   explicit Test(std::string name, bool value) :
      name_(std::move(name)), value_(value)
   {}
   
   bool value() const { return value_; }
   
   friend std::ostream& operator<<(std::ostream& os, const Test& t )
   {
      os << "TEST ==> " << t.name_ << " ===== PASSED ==> " << std::boolalpha << t.value_;
      return os;
   }
   
};

/// collection of all the tests to run
std::vector<Test> tests;

///
/// Tests implementation
///

inline
void test_MyList_insert() {
   tests.emplace_back("test_MyList_insert", (MyList<int>{1,2,3,4} == MyList<int>{1,2,3,4}));
}

inline
void test_MyList_reverse() {
   tests.emplace_back("test_MyList_reverse", (MyList<int>{1,2,3,4}.reverse() == MyList<int>{4,3,2,1}));
}

inline
void test_MyList_filter() {
   
   auto f = [](int val){
      return val%2 == 0;
   };
   
   tests.emplace_back("test_MyList_filter", (MyList<int>{1,2,3,4}.filter(f) == MyList<int>{2,4}));
}

inline
void test1_MyList_map() {
   
   auto f = [](const std::string& val){
      return (int)val.length();
   };
   
   tests.emplace_back("test1_MyList_map",
                      (MyList<std::string>{"foo","bar","baz","boom"}.map<int>(f) == MyList<int>{3,3,3,4}));
}

inline
void test2_MyList_map() {
   
   auto f = [](std::string val)
   {
      //string to upper case
      std::transform(val.begin(), val.end(), val.begin(), ::toupper);
      return val;
   };
   
   tests.emplace_back("test2_MyList_map",
                      (MyList<std::string>{"foo","bar","baz"}.map<std::string>(f) == MyList<std::string>{"FOO","BAR","BAZ"}));
}

inline
void test1_MyList_foldLeft() {
   
   auto f = [](const std::string& val){
      return (int)val.length();
   };
   
   tests.emplace_back("test1_MyList_foldLeft",
                      (MyList<std::string>{"foo","bar","baz","boom"}.foldLeft<int>(f) == 13));
   
}

inline
void test2_MyList_foldLeft() {
   
   auto f = [](const std::string& val){
      return val;
   };
   
   tests.emplace_back("test2_MyList_foldLeft",
                      (MyList<std::string>{"foo","bar","baz"}.foldLeft<std::string>(f) == "foobarbaz"));
}

inline
void test3_MyList_foldLeft() {
   
   auto f = [](int val){
      return -val;
   };
   
   tests.emplace_back("test3_MyList_foldLeft",
                      (MyList<int>{1,2,3}.foldLeft<int>(f) == -6));
   
   tests.emplace_back("test3_MyList_foldLeft_bis",
                      (MyList<int>{10,2,3}.foldLeft<int>(f) == -15));
}

///
/// final validation
///
void test_validation() {
   
   //run the battery of tests
   test_MyList_insert();
   test_MyList_filter();
   test1_MyList_map();
   test2_MyList_map();
   test1_MyList_foldLeft();
   test2_MyList_foldLeft();
   test3_MyList_foldLeft();
   
   //verify
   bool res = true;
   for(const auto& test : tests) {
      std::cout << test  << "\n";
      res &= test.value();
   }
   
   //no test should fail
   assert(res == true);
}


#endif /* MyList_test_h */
