#include <iostream>
#include <mpi.h>
#include <random>
#include <vector>
#include <algorithm>

/*
 *	Parallel ranking
 * */

struct node_t { 
	int id; 
	int sched_rank;

	bool operator < (const node_t& x) const {
		return sched_rank < x.sched_rank;
	}
};

int generate_rank() 
{
	std::random_device rd;
	std::uniform_int_distribution<int> dist(0, 9);
	return dist(rd);
}

void serialize_node_info(MPI_Datatype& data) 
{
	const int nitems=2;
	int blocklengths[2] = {1,1};
	MPI_Datatype types[2] = {MPI_INT, MPI_INT};
	MPI_Aint     offsets[2];

	offsets[0] = offsetof(node_t, id);
	offsets[1] = offsetof(node_t, sched_rank);

	MPI_Type_create_struct(nitems, blocklengths, offsets, types, &data);
	MPI_Type_commit(&data);
}

int main(int argc, char** argv) {

	int size, rank;


	MPI_Init(NULL,NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	MPI_Datatype mpi_node_type;
	serialize_node_info(mpi_node_type);

	node_t n;
	n.id = rank;
	n.sched_rank = generate_rank();	

	//std::cout << "Process rank = " << n.id << " my local scheduling id = " << n.sched_rank << std::endl;
	//sent to process 0 in order to decide the scheduling priority.
	
	std::vector<node_t> res;
	if( rank == 0 )
		res.resize( size );

	MPI_Gather(&n, 1, mpi_node_type, &res[0], 1, mpi_node_type, 0, MPI_COMM_WORLD);

	if( rank == 0 )
	{
		std::sort( res.begin(), res.end() );

		for(const auto& d : res )
			std::cout << " id = " << d.id << " shed rank = " << d.sched_rank << std::endl;
	}

	MPI_Type_free(&mpi_node_type);
	MPI_Finalize();

	return 0;
}
