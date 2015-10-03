#include <thread>

#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <chrono>
#include <future>

//block size under with do not go
std::size_t block = 1000;

/*
 * Generate a vector of distict elements of size N. Where elements are into [0,N).
 * Shuffle result afterwards and return vector
 * */
template<typename T>
std::vector<T>generate(std::size_t& size)
{
   std::vector<T> v;
   v.reserve(size);
   
   for (unsigned i = 0; i<size; ++i)
      v.push_back(i);
   
   std::random_shuffle(v.begin(), v.end());
   return v;
}

/*
 * Given a range [begin,end), and a variable where deposit the result.
 * Perform the sum of all elements into the range passed and store result in variable passed
 *
 * @note: see std::accumulate algorithm
 * */
template<typename I, typename T>
void range_accumulate(I begin, I end, T& res)
{
   res = std::accumulate(begin, end, res);
}

const unsigned compute_number_of_threads(unsigned len)
{
   const unsigned physical_thread_number = std::thread::hardware_concurrency();
   const unsigned min_split_value = 25;
   const unsigned needed_threads = (len + min_split_value - 1) / min_split_value;
   const unsigned available_threads = physical_thread_number != 0 ? physical_thread_number : 2;
   
   std::cout << "Range size = " << len << std::endl;
   std::cout << "Available physical thread = " << available_threads << std::endl;
   std::cout << "Needed threads for this task = " << needed_threads << std::endl;
   
   const unsigned num_threads = std::min(available_threads, needed_threads);
   return num_threads;
}

/*
 * Parallel accumulator spawing a fixed number of std::threads
 * */
template<typename I, typename T>
T parallel_acc(I begin, I end, T init)
{
   auto len = std::distance(begin, end);
   
   if (len == 0)
      return init;
   
   //if(len < 1000)
   //    return std::accumulate(begin,end,0);
   
   const unsigned num_threads = compute_number_of_threads(len);
   std::vector<T> res(num_threads);
   std::vector<std::thread> threads(num_threads -1);
   
   //split and sum
   const unsigned block_size = len / num_threads;
   
   std::cout << "Block size = " << block_size << std::endl;
   
   I block_start = begin;
   for (unsigned i = 0; i < (num_threads-1); ++i)
   {
      I block_end = block_start;
      std::advance(block_end, block_size);
      
      threads[i] = std::thread(range_accumulate<I, T>, block_start, block_end, std::ref(res[i]));
      block_start = block_end;
   }
   
   range_accumulate(block_start, end, res[num_threads-1]);
   
   //join
   for (auto& t : threads)
      if (t.joinable())
         t.join();
   
   //reduce all
   return std::accumulate(res.begin(), res.end(), init);
}

/*
 * using std::async (accordingly with standard cpp it implements a thread pool)
 * It encourages to use task based design
 * */
template<typename I>
long async_accumulate(I begin, I end)
{
   const auto len = end - begin;
   if (len <= block)
      return std::accumulate(begin, end, 0);
   
   I mid = begin + len / 2;
   auto handle = std::async(async_accumulate<I>, mid, end);
   int sum = async_accumulate<I>(begin, mid);
   return sum + handle.get();
   
}

/*
 * Second version to spawn threads using std::async
 * */

template<typename I>
long async_accumulate_2(I begin, I end)
{
   auto len = std::distance(begin, end);
   
   if (len == 0)
      return 0;
   
   const unsigned num_threads = compute_number_of_threads(len);
   std::vector<std::future<long>> res(num_threads-1);
   const unsigned block_size = len / num_threads;
   
   auto f =[](I b, I e, long init) { return std::accumulate(b,e,init); };
   
   auto start_block = begin;
   for(unsigned i =0; i < num_threads-1; ++i)
   {
      auto end_block  = start_block;
      std::advance(end_block, block_size);
      res[i] = std::async(f, start_block, end_block, 0);
      start_block = end_block;
   }
   
   long sum = f(start_block, end, 0);
   for(auto& f : res )
      sum += f.get();
   
   return sum;
}

int main(int argc, char** argv)
{
   if (argc == 3)
   {
      std::size_t size  = std::atoi(argv[1]);
      block = std::atoi(argv[2]);
      
      //std::size_t size = _tstoi(argv[1]);
      auto v = generate<long>(size);
      
      using I = std::vector<long>::iterator;
      using T = int;
      
      int res_seq = 0;
      int res_par = 0;
      int res_async = 0;
      int res_async_2 = 0;
      
      auto b_seq = std::chrono::high_resolution_clock::now();
      range_accumulate(v.begin(), v.end(), res_seq);
      auto e_seq = std::chrono::high_resolution_clock::now();
      
      auto b_par = std::chrono::high_resolution_clock::now();
      res_par = parallel_acc(v.begin(), v.end(), 0);
      auto e_par = std::chrono::high_resolution_clock::now();
      
      auto b_async = std::chrono::high_resolution_clock::now();
      res_async = async_accumulate(v.begin(), v.end());
      auto e_async = std::chrono::high_resolution_clock::now();
      
      auto b_async_2 = std::chrono::high_resolution_clock::now();
      res_async_2 = async_accumulate_2(v.begin(), v.end());
      auto e_async_2 = std::chrono::high_resolution_clock::now();
      
      
      using unit_time_t = std::chrono::microseconds;
      std::string str_unit = "us";
      std::cout << std::endl << "Final Result" << std::endl
      
      << "single thread sum = " << res_seq
      << " time taken = "
      << std::chrono::duration_cast<unit_time_t>(e_seq - b_seq).count() << str_unit << std::endl
      
      << "parallel sum = " << res_par
      << " time taken = "
      << std::chrono::duration_cast<unit_time_t>(e_par - b_par).count() << str_unit << std::endl
      
      << "async sum = " << res_async
      << " time taken = "
      << std::chrono::duration_cast<unit_time_t>(e_async - b_async).count() << str_unit << std::endl
      
      << "async2 sum = " << res_async_2
      << " time taken = "
      << std::chrono::duration_cast<unit_time_t>(e_async_2 - b_async_2).count() << str_unit << std::endl
      
      << std::endl;
   }
   else
   {
      std::cout << "Correct usage is -> ./a.out <size> <block_chunk>" << std::endl;
   }
   
   return 0;
}


