#include <thread>
#include <iostream>

void hello()
{
	std::cout << "Hello World! thread id = " << std::this_thread::get_id() << std::endl;
}

int main(int argc, char** argv)
{
	std::thread t(hello);
	t.join(); //comment this to have sigabort... you must care about your threads (solution use a scoped thread)
	return 0;
}
