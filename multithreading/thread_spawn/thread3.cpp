#include "scoped_thread.h"
#include <iostream>

int main(int argc, char** argv)
{
	auto f = [](){ std::cout << "ID of this thread = " <<std::this_thread::get_id() << std::endl; };
	scoped_thread th(std::thread{f});
	return 0;
}
