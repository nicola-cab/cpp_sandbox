// FunctionalCpp.cpp : Defines the entry point for the console application.
//

#include <list>

#include "Launcher.h"               //benchmark header
#include "STL_algorithm.h"           //list of stl algorithms I have replaced using tail recursion
#include "TailRecursive_Algorithm.h" //list of tail recursive algorithms implemented
#include "BenchMark.h"

#include "list_functions.h"

//benchmark classes
using test::Launcher;
using test::RandomRangeGenerator;
using test::Print;

//defines the type of benchmarks object I need.
//each object runs the algorithm and is specialized 
//to cope with the result type of each function
using benchmark::Run;
using benchmark::SimpleRunner;
using benchmark::PairItRunner;
using benchmark::SimpleRunnerFaultTollerante;

//size of the sample used (todo-> understand if tail recursion optimizaton allows me to pick a very big number as size of the array)
const std::size_t SIZE = 10000;

template<typename I>
void compare_list_creation(I b, I e)
{
   using time_frame_t = std::chrono::nanoseconds;
   const std::string unit = "ns";
   
   //functional list vs std::list (bechmark only insertion)
   auto b1 = std::chrono::system_clock::now();
   auto list = list_function::fromIterator(b,e);
   auto e1 = std::chrono::system_clock::now();
   
   auto b2 = std::chrono::system_clock::now();
   std::list<int> std_list { b,e };
   auto e2 = std::chrono::system_clock::now();
   
   std::cout << "Persistent List creation has taken = "
   << std::chrono::duration_cast<time_frame_t>(e1 - b1).count()
   << unit << std::endl;
   
   std::cout << "std::list creation has taken = "
   << std::chrono::duration_cast<time_frame_t>(e2 - b2).count()
   << unit << std::endl;

}

int main(int argc, char* argv[])
{
   //compare performances of a couple of tail recursive algorithm re-implemented by me 
   //with the std::stl implementation of my compiler
   using type_collection_t = int;

   RandomRangeGenerator<type_collection_t> r;
   auto v = r.gen(0, 10, SIZE);  
   auto v1 = r.gen(0, 10, SIZE); //in case of range comparison algorithms I have a second vector ready to use

   using V = decltype(v);

   using return_type_const_iterator = V::const_iterator;
   using return_type_iterator = V::iterator;
   using return_type_scalar = type_collection_t;
   using return_type_pair_it = std::pair < V::const_iterator, V::const_iterator > ;
   

   Launcher<return_type_const_iterator> b_cit;
   Launcher<return_type_scalar> b_scalar;
   Launcher<return_type_pair_it> b_pair_it;
   Launcher<bool> b_bool;

   Run<SimpleRunner> run;      //manages return of Iterators and scalar types
   Run<PairItRunner> pair_run; //manages return of pair of iterators
   Run<SimpleRunnerFaultTollerante> srun; //runner that excepts to have a pair of iterators that point to end 
   
   //Print p;
   //p.print(v.begin(), v.end());

   //stl algoritms 
   stl_find f;
   stl_find_if fif;
   stl_find_if_not fifn;
   stl_find_first_of ffo;
   stl_adject_find adjf;
   stl_count c;
   stl_count_if cif;
   stl_accumulate acc;
   stl_mismatch m;
   stl_equal e;
   stl_advance a;
   stl_distance d;
   stl_is_permuatation isp;
   stl_search s;
   stl_min_element min;
   stl_adj_difference adj_d;
   stl_inner_product in_prod;
   stl_partial_sum p_sum;

   //my personal algorithm
   tail_find tf;
   tail_find_if tfif;
   tail_find_if_not tfifn;
   tail_find_of tffo;
   tail_adject_find tadjf;
   tail_count tc;
   tail_count_if tcif;
   tail_accumulate tacc;
   tail_mismatch tm;
   tail_equal te;
   tail_advance ta;
   tail_distance td;
   tail_is_permuatation tisp;
   tail_search ts;
   tail_min_element tmin;
   tail_adj_difference tadj_d;
   tail_inner_product tin_prod;
   tail_partial_sum tp_sum;


   //benchmarks
   
   //find 
   run(b_cit, f, tf, v.cbegin(), v.cend(), 8);

   //find_if
   auto unary = [](const V::value_type& value) { return (value % 2); };
   run(b_cit, fif, tfif, v.cbegin(), v.cend(), unary);

   //find if not
   run(b_cit, fifn, tfifn, v.cbegin(), v.cend(), unary);

   //find the first of 
   run(b_cit, ffo, tffo, v.cbegin(), v.cend(), v1.cbegin(), v1.cend());

   //find the first of 
   run(b_cit, adjf, tadjf, v.cbegin(), v.cend());

   //count elements
   run(b_scalar, c, tc, v.cbegin(), v.cend(), 5);

   //count if
   auto isEven = [](const V::value_type& v) { return v % 2; };
   run(b_scalar, cif, tcif, v.cbegin(), v.cend(), isEven);

   //accumulate
   auto sumBin = [](const V::value_type& v1, const V::value_type& v2) { return v1 + v2; };
   run(b_scalar, acc, tacc, v.cbegin(), v.cend(), 0, sumBin);

   //mismatch
   pair_run(b_pair_it, m, tm, v.cbegin(), v.cend(), v1.cbegin()); //reuse v1

   //equal
   run(b_bool, e, te, v.cbegin(), v.cend(), v.cbegin()); //equal
   run(b_bool, e, te, v.cbegin(), v.cend(), v1.cbegin()); //not equal

   //advance
   run(b_cit, a, ta, v.begin(), v.size() / 2);

   //distance
   run(b_scalar, d, td, v.begin(), v.begin() + v.size() / 2);

   //is permutation
   run(b_scalar, isp, tisp, v.begin(), v.end(), v1.begin());

   //search
   srun(b_cit, s, ts, v.cend(), v1.cend(), v.cbegin(), v.cend(), v1.cbegin(), v1.cend());

   //min element
   run(b_cit, min, tmin, v.begin(), v.end());

   //adject_diff 
   std::vector<int> vv = { 1, 2, 3, 5 };
   std::vector<type_collection_t> v2;
   v2.resize(vv.size());
   run(b_cit, adj_d, tadj_d, vv.begin(), vv.end(), v2.begin());

   //inner_product
   run(b_scalar, in_prod, tin_prod, v.begin(), v.end(), v.begin(), 1);

   //partial sum
   run(b_cit, p_sum, tp_sum, v.begin(), v.end(), v.begin());

   
   //compare std::list vs crafted one
   compare_list_creation(v.begin(), v.end());


	return 0;
}

