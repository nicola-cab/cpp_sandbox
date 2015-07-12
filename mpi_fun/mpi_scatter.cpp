#include <iostream>
#include <mpi.h>

const int SIZE = 2;

int main(int argc, char** argv) {
	int n_tasks, rank;
	int matrix[SIZE][SIZE] = { {0,1}, {2,3} };
	int buff[SIZE] = {0};

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&n_tasks);
	
	if(n_tasks == SIZE ) {
 		MPI_Scatter(
				matrix,SIZE,MPI_INT, //data to scatter
				buff,SIZE,MPI_INT,  //buff where deposit scattered data
				0,MPI_COMM_WORLD);  

		std::cout << "Process with rank = " << rank << std::endl;
		for( auto i = 0; i < SIZE; ++i )
			std::cout << buff[i] << " ";
		std::cout << std::endl;

	} 
	else {
		std::cout << "Error since number of processors is : " 
			<< n_tasks << " and you programs requires " << SIZE << std::endl; 
	}

	MPI_Finalize();

	return 0;
}
