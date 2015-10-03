#include <iostream>
#include <map>
#include <future>
#include <mutex>
#include <vector>
#include <thread>

std::mutex m;
using lock = std::lock_guard<std::mutex>;
std::map<std::thread::id,bool> ids;

void f(unsigned i)
{
    lock lk{m};
	 auto id = std::this_thread::get_id();

	 std::cout << "thread #"<<i<< " id = " << id << std::endl;	 
	 ids.insert(std::make_pair(id, false));
}


int main(int argc, char** argv)
{
	std::vector<std::future<void>> futures(10);
	for(unsigned i = 0; i<10; ++i)
		futures[i] = std::async(std::launch::any, f, i);

	for(auto&f: futures )
		f.wait();

	std::cout << "Actual thread spawned = " << ids.size() <<std::endl;
	for(const auto& v : ids)
	{
		std::cout << v.first << std::endl;
	}

    return 0;
}
