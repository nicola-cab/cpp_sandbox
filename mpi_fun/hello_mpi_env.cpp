#include <iostream>
#include <mpi.h>

int main(int argc, char** argv) {
	int size, rank, len;
	char hostname[MPI_MAX_PROCESSOR_NAME] = {0};

	auto rc = MPI_Init(&argc, &argv);
	if( rc != MPI_SUCCESS ) {
		std::cout << "Error MPI_Init" <<std::endl;
		MPI_Abort(MPI_COMM_WORLD, rc);
	}
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Get_processor_name(hostname, &len);
	std::cout << "Number of tasks " << size << " I am the task " << rank << " name is " << hostname << std::endl; 
	MPI_Finalize();
	return 0;
}
