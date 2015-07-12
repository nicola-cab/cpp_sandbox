#include <iostream>
#include <vector>
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

	MPI_Comm_group(MPI_COMM_WORLD,&orig_group);

	//split processes in two groups
	if( rank < n_tasks/2 ){

	  	std::vector<int> ranks;
		for( auto i=0;i<n_tasks/2;++i)
			ranks.push_back(i);	

		MPI_Group_incl(orig_group, n_tasks/2, &ranks[0], &new_group);
	}
	else {
		std::vector<int> ranks;
		for( auto i=n_tasks/2; i<n_tasks ; ++i)
			ranks.push_back(i);	

		MPI_Group_incl(orig_group, n_tasks/2, &ranks[0], &new_group);
	}

	out = rank*2;

	MPI_Comm_create(MPI_COMM_WORLD, new_group, &new_comm);
	MPI_Allreduce(&out, &final_out, 2, MPI_INT, MPI_SUM, new_comm);
	MPI_Group_rank (new_group, &new_rank);

	std::cout << "My rank is = " << rank << " new rank is = " << new_rank << " recvbuf = " << final_out << std::endl;

	MPI_Finalize();
	return 0;
}
