#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

using lock_t = std::lock_guard<std::mutex>;
std::mutex m;

int main(int argc, char** argv) {

	auto f = [](unsigned i){
		lock_t _l( m );
		std::cout << "thread with rank = " << i << std::endl;
	};

	std::vector<std::thread> threads;
	auto size = std::thread::hardware_concurrency();
	for( auto i = 0; i<size; ++i)
		threads.push_back( std::thread( std::move(f), i));
	for(auto& t: threads )
		t.join();
	return 0;
}
