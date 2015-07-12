#include <iostream>
#include <mpi.h>
#include <vector>
#include <random>

const int N = 10;

std::vector<int> gen() {

	std::vector<int> v;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(1, 6);
	for (int n=0; n<N; ++n)
		v.push_back(dis(gen)); 
	return v;
}

template<typename I>
int sum( I b, I e ){
	auto s = 0;
	while( b!=e ) {
		s += *b++;
	}
	return s;
}

template<typename I>
float sq_diff(I b, I e, float m){
	float l_sq = 0;
	while( b != e ){
		auto v = *b++;
		l_sq += std::pow( v-m , 2);
	}
	return l_sq;
}

template<typename I>
void print( I b, I e){
	while( b!=e )
		std::cout << *b++ << " -> ";
	std::cout << std::endl;
}

int main(int argc, char** argv) {
	int size, rank;

	MPI_Init(NULL,NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	//random number for my phenomena
	auto v = gen();
	auto l_sum = sum(v.begin(), v.end());
	//print(v.begin(), v.end());

	//std::cout << "Rank " << rank << " l_sum = " << l_sum << std::endl;

	//send to all the sums for their local phenomenas and sum all together
	auto g_sum = 0;
	MPI_Allreduce(&l_sum,&g_sum,1,MPI_INT,MPI_SUM,MPI_COMM_WORLD);

	std::cout << "Rank " << rank << " g_sum = " << g_sum << std::endl; 
	
	//compute the mean
	float mean = (float)g_sum/(N*size);
	//square diff between single element and mean
	auto diff = sq_diff(v.begin(), v.end(), mean);	

	//send to process 0 the global diff 
	float g_diff = 0;
	MPI_Reduce(&diff, &g_diff, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);

	if( rank == 0 ){
		//compute deviation
		auto stddev = std::sqrt(g_diff/(N*size));
		std::cout << "Mean = " << mean << " Stddev = " << stddev << std::endl;
	}
	MPI_Finalize();
	return 0;
}
