#include <thread>
#include <iostream>

void hello()
{
	std::cout << "Hello World! thread id = " << std::this_thread::get_id() << std::endl;
}

int main(int argc, char** argv)
{
	std::thread t(hello);
	t.join(); 
	return 0;
}
