#include <iostream>
#include <mpi.h>
#include <vector>
#include <random>

const int N = 100;

 std::vector<int> random_gen()
{
	std::vector<int> v;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(1, 6);
	for (int n=0; n<N; ++n)
		v.push_back(dis(gen)); 
	return v;
}

template<typename I>
void print(I b, I e){
	std::cout << "vector values : " << std::endl;
	while(b!=e) {
		std::cout << *b++ << " -> ";
	}
	std::cout << std::endl;

}

template<typename T>
float return_avg( const std::vector<T>& v) {
	auto size = v.size();
	float avg = 0;
	for(const auto& e: v){
		avg+=e;
	}
	return avg/size;
}

int main( int argc, char** argv) {
	int size, rank;
	std::vector<int> v ;

	int c = 0;
	if( argc == 2 )
		c = std::atoi(argv[1]);

	MPI_Init(&argc, &argv);
  	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if(rank == 0) {
		v = random_gen();
		//std::cout << "All data " << rank << std::endl;
		//print(v.begin(),v.end());
	}
	
	std::vector<int> partial(N/size); //assuming the splitting is always possible and there is no rest.

	MPI_Scatter(&v[0], partial.size(),MPI_INT, 		//init data and split
					&partial[0],partial.size(),MPI_INT, //buff where deposit
					0,MPI_COMM_WORLD); 
	

	//compute avg
	auto avg = return_avg( partial );
	//std::cout << "rank " << rank << " avg = " << avg << std::endl;

	//gather back the data
	std::vector<float> res(size);	

	if( c == 0 ) 
	{
		MPI_Gather(&avg,1,MPI_FLOAT,&res[0],1,MPI_FLOAT,0,MPI_COMM_WORLD);
	}
	else if( c == 1) 
	{
		MPI_Allgather(&avg,1,MPI_FLOAT,&res[0],1,MPI_FLOAT,MPI_COMM_WORLD);
	}

	if( c == 0 && rank == 0 ) 
	{
		auto final_avg = return_avg( res );
		std::cout << "Final avg is = " << final_avg <<std::endl;

		auto all_avg = return_avg( v );
		std::cout << "Avg computed on entire collection = " << all_avg << std::endl;
	}
	else 
	{
		auto final_avg = return_avg( res );
		std::cout << "Final avg is = " << final_avg <<std::endl;
	}

	MPI_Finalize();	
	return 0;
}
