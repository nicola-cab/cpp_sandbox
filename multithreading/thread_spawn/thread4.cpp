#include <iostream>
#include <string>
#include <vector>
#include <mutex>

#include "scoped_thread.h"
#include "fancy_object.h"

std::mutex _m;

class Object
{
	public:
		void f(int id, std::string s)
		{
			std::lock_guard<std::mutex> lk{_m};
			std::cout << "id # = " << id
				<< " - Functional object - I am a thread with ID = " << std::this_thread::get_id() 
				<< " custom msg = "<< s <<std::endl;
		}
};

struct functionalObject
{
	void operator()(int id, std::string s) const
	{
        std::lock_guard<std::mutex> lk{_m};
        std::cout << "id # = " << id
            << " - Functional object - I am a thread with ID = " << std::this_thread::get_id()
            << " custom msg = "<< s <<std::endl;
	}
};

void fnt(int id, std::string s)
{
    std::lock_guard<std::mutex> lk{_m};
    std::cout << "id # = " << id
			<< " - Functional object - I am a thread with ID = " << std::this_thread::get_id()
			<< " custom msg = "<< s <<std::endl;
}

auto lambda_f = [](int id, std::string s)
{
    std::lock_guard<std::mutex> lk{_m};
    std::cout << "id # = " << id
			<< " - Functional object - I am a thread with ID = " << std::this_thread::get_id()
			<< " custom msg = "<< s <<std::endl;
};

void consume_fancy_object(A a)
{
    std::lock_guard<std::mutex> lk{_m};
    std::cout << a << std::endl;
}

void spawn_using_threads()
{
	std::vector<std::thread> threads; 

	threads.emplace_back(fnt,0,"Hi");
	threads.emplace_back(fnt,1,"Salut");
	threads.emplace_back(fnt,2,"Ciao");
	threads.emplace_back(fnt,3,"Hola");

//	threads.emplace_back(lambda_f,1,"Salut");
//	threads.emplace_back(functionalObject{},2,"Ciao");
//	threads.emplace_back(&Object::f, Object{},3,"Hola");
	//threads.emplace_back(consume_fancy_object,A{});

	for(auto& t: threads )
		t.join();

}

void spawn_using_scoped_threads()
{
	std::vector<scoped_thread> threads; 
	
	threads.emplace_back(std::thread(fnt,0,"Hi"));
	threads.emplace_back(std::thread(lambda_f,1,"Salut"));
	threads.emplace_back(std::thread(functionalObject{},2,"Ciao"));
	threads.emplace_back(std::thread(&Object::f, Object{},3,"Hola"));
//	threads.emplace_back(std::thread(consume_fancy_object,A{})); 
}

int main(int argc, char** argv)
{
	if(argc == 2)
	{
		auto choice = std::atoi(argv[1]);
		switch (choice)
		{
			case 0:
				spawn_using_threads();
				break;
			default:
				spawn_using_scoped_threads();
				break;
		}
	}
	return 0;
}
