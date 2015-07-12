#include <iostream>
#include <mpi.h>

const int TASKS = 2;

int main(int argc, char** argv){
	int rank, n_tasks;
	int new_rank; 
	int out, final_out = 0;
	MPI_Group orig_group, new_group;
	MPI_Comm new_comm;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &n_tasks);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if( n_tasks != TASKS ){
		std::cout << " task available " << n_tasks << " != than task available " << TASKS << std::endl;
		MPI_Finalize();
		exit(0);
	}

	MPI_Comm_group(MPI_COMM_WORLD,&orig_group);

	//split process in two groups
	if( rank < TASKS/2 ){
		int ranks[1] = {0};
		MPI_Group_incl(orig_group, TASKS/2, ranks, &new_group);
	}
	else {
		int ranks[1]={1};
		MPI_Group_incl(orig_group, TASKS/2, ranks, &new_group);
	}

	if( rank == 0 ) {
		out = 10;
	}
	else if( rank == 1) {
		out = 11;
	}

	MPI_Comm_create(MPI_COMM_WORLD, new_group, &new_comm);
	MPI_Allreduce(&out, &final_out, 2, MPI_INT, MPI_SUM, new_comm);
	MPI_Group_rank (new_group, &new_rank);

	std::cout << "My rank is = " << rank << " new rank is = " << new_rank << " recvbuf = " << final_out << std::endl;

	MPI_Finalize();
	return 0;
}
