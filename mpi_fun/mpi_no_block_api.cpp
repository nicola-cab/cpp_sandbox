#include <iostream>
#include <mpi.h>

int main( int argc, char** argv) {
	int num_task, rank;
	int tag1 = 0, tag2=1;
	int resp[2] = {0};
	MPI_Request reqs[4];
	MPI_Status stas[4];

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &num_task);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	auto prev = rank -1;
	auto next = rank +1;
	
	if( rank == 0 )
		prev = num_task -1;

	if( rank == num_task-1 )
		next = 0;

	MPI_Irecv(&resp[0],1,MPI_INT,prev,tag1,MPI_COMM_WORLD,&reqs[0]);
	MPI_Irecv(&resp[1],1,MPI_INT,next,tag2,MPI_COMM_WORLD,&reqs[1]);

	MPI_Isend(&rank, 1, MPI_INT, prev, tag2, MPI_COMM_WORLD, &reqs[2]);
	MPI_Isend(&rank, 1, MPI_INT, next, tag1, MPI_COMM_WORLD, &reqs[3]);

	for( auto i=0; i<10000; ++i);

	MPI_Waitall(4,reqs,stas);
	std::cout << "Process "<< rank << " prev -> " << resp[0] << " next ->" << resp[1] << std::endl;

	MPI_Finalize();	
	
	return 0;
}
