#include <iostream>
#include <thread>
#include <string>
#include <sstream>
#include <fstream>


enum class Mode 
{
	SIMPLE, COMPLEX_JOIN, COMPLEX_DETACH 
};

void fnt( int a )
{
	std::cout << a <<  std::endl;
}

void complex_fnt( std::string& s)
{
	std::stringstream buff;	
	std::ofstream f("./test.txt");

	if(f.is_open())
	{
		buff << s << std::this_thread::get_id() << std::endl;
		f << buff.str();
		f.close();
	}
}

void spawn_task(Mode choice, int x = 0)
{
	switch (choice)
	{
		case Mode::SIMPLE:
		{
			std::cout << "-- Detach Simple --" << std::endl;
			std::thread t(fnt,x);
			t.detach();
		}
		break;

		case Mode::COMPLEX_JOIN:
		{
			std::cout << "-- Join a comple task --" << std::endl;
			
			std::string s = "Hello world ";

			//complex object passed by reference	
			std::thread t(complex_fnt, std::ref(s));

			//assure job is done and return
			t.join(); 
		}
		break;

		case Mode::COMPLEX_DETACH:
		{
			std::cout << "-- Detach a comple task --" << std::endl;

			std::string s = "Hello world ";
			//complex object passed by reference	
			std::thread t(complex_fnt, std::ref(s));

			//detach the thread... if the program exits before the threads completes its job, no job is done 
			t.detach(); 

			//wait 5ms in order to assure that file is written on disk
			std::this_thread::sleep_for(std::chrono::nanoseconds(x));
		}
		break;


		default:
			std::cout << "Option not recognized" << std::endl;
		break;
	}
}


int main(int argc, char** argv)
{
	if( argc > 1 )
	{
		int choice = std::atoi(argv[1]);

		int x = 1;
		if(argc == 3)
		{
			x = std::atoi(argv[2]);
		}


		switch(choice)
		{
			case 0:
				spawn_task(Mode::SIMPLE,x);
				break;

			case 1:
				spawn_task(Mode::COMPLEX_JOIN);
				break;

			case 2:
			{
				//std::cout <<"Sleep " << x << " ns" << std::endl;
				spawn_task(Mode::COMPLEX_DETACH, x);
			}
			break;

			default:
				break;
		}
	}
	else 
	{
		std::cout << "Provide your choice. Usage: ./a.out (0/1/2)" << std::endl;
	}
	
	return 0;
}
