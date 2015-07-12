#include <iostream>
#include <mpi.h>
#include <random>
#include <vector>

std::vector<int> random_gen()
{
	std::vector<int> v;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(1, 6);
				 
	for (int n=0; n<10; ++n)
		v.push_back(dis(gen));

	return v;
}

void mpi_reduce(int rank, int size, int root, bool flag) {
	if( rank >= size || root >= size )
		return;

	auto v = random_gen();	
	int sum = 0;
	int res = 0;
	for(const auto& _elem : v ) {
//		std::cout << "Process with rank = " << rank << " val = " << _elem << std::endl;
		sum += _elem;
	}
	std::cout << "Process " << rank+1 << "/" << size << std::endl;
	std::cout <<" Sum = " << sum << std::endl;	

	if( flag )
		MPI_Reduce(&sum, &res,1,MPI_INT,MPI_SUM,root,MPI_COMM_WORLD);
	else 
		MPI_Allreduce(&sum,&res,1,MPI_INT,MPI_SUM,MPI_COMM_WORLD);


//	if( rank == root ) {
	std::cout << "Process rank = " << rank << " has result = " << res << std::endl;
//	}
}

int main(int argc, char** argv) {
	int rank, tasks;
	int choice; 
	
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD, &tasks);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	mpi_reduce(rank, tasks, 0, true);  //mpi_reduce
	mpi_reduce(rank, tasks, 0, false);  //mpi_reduce_all

	MPI_Finalize();
	return 0;
}
