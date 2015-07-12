 /*
  * Open Mpi first test application
  * */

#include <iostream>
#include <string>

//mpi support
#include <mpi.h>

//std thread support
#include <thread>
#include <vector>
#include <mutex>

using lock_t = std::lock_guard<std::mutex> ;
std::mutex m;

void mpi_run( int argc, char** argv )
{
	int rank, size, len;
	char version[MPI_MAX_LIBRARY_VERSION_STRING] = {0};

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Get_library_version(version, &len);
 
	 std::cout << "Hello world, I am " 
		 << (int)rank+1 << " / " 
		 << size
		 << " open mpi version :" << version << std::endl;
    
	 MPI_Finalize();
}

void thread_run(){

	std::cout << std::endl << std::endl << "Threading model : " << std::endl;

	auto f = [](int i){ 
		lock_t _l(m);
		std::cout << "thread = " << i << std::endl;
	};

	auto hw = std::thread::hardware_concurrency();
	std::vector<std::thread> threads;
  	for( auto i = 0; i < hw ; ++i)
		threads.push_back( std::thread( std::move(f), i));	

	for( auto& t: threads )
		t.join();
}

int main(int argc, char* argv[])
{
	mpi_run(argc,argv);
	thread_run();
	return 0;
}
