#include <iostream>
#include <mpi.h>
#include <string>

#include <chrono>

std::string prepare_buff(int source, int tag, int& data_count) {
    MPI_Status status;
    MPI_Probe(source, tag, MPI_COMM_WORLD, &status);
    MPI_Get_count(&status, MPI_CHAR, &data_count);
    if( data_count < 0 )
    {
        std::cout << "Aborted since there has been an error reading bytes from this communication. Value is = "
        << data_count
        << std::endl;
        MPI_Abort(MPI_COMM_WORLD, -1);
    }
    //auto b = std::chrono::high_resolution_clock::now();
    std::string _s(' ',data_count);
    //_s.reserve(data_count);
    //auto e = std::chrono::high_resolution_clock::now();
    //std::cout << "Time passed: "
    //    << std::chrono::duration_cast<std::chrono::nanoseconds>(e-b).count() << " ns"<< std::endl;
    return _s;
}

int main(int argc, char** argv) {
	int size, rank;
    std::string out = "Hello World";
    
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if( rank == 0 ){
		//process 0
        
        std::cout << "process = " << rank << std::endl;
        out += " I am process 0";
        
        MPI_Status status;
        int data_count =0;
        
        auto rc = MPI_Send(out.c_str(), out.size(), MPI_CHAR, 1,0, MPI_COMM_WORLD);
        std::cout << "Process 0: sent to process 1" << std::endl;

        std::cout << "Process 0: waiting ... " << std::endl;
        auto s = prepare_buff(1,0, data_count);
		rc = MPI_Recv(&s[0], data_count, MPI_CHAR, 1, 0, MPI_COMM_WORLD, &status);
        std::cout << "Process 0: string received " << s << std::endl;
	}
	else if( rank == 1) {
		//process 1
        std::cout << "process = " << rank << std::endl;
        out += " I am process 1";

        MPI_Status status;
        int data_count = 0;

        auto s = prepare_buff(0,0, data_count);
        std::cout << "Process 1: waiting ..." << std::endl;
        auto rc = MPI_Recv(&s[0], data_count, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
        std::cout << "Process 1: string received :" << s << std::endl;
        
        rc = MPI_Send(out.c_str(), out.size(), MPI_CHAR, 0, 0, MPI_COMM_WORLD);
        std::cout << "Process 1: sent to process 0" << std::endl;
	}
    
    MPI_Finalize();
    
	return 0;
}
