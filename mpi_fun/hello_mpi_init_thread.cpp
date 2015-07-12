#include <iostream>
#include "mpi.h"

int main(int argc, char** argv) {
	int provided, claimed;
	 
	/*** 
	 * Select one of the following
	 * MPI_Init_thread( 0, 0, MPI_THREAD_SINGLE, &provided );
	 * MPI_Init_thread( 0, 0, MPI_THREAD_FUNNELED, &provided );
	 * MPI_Init_thread( 0, 0, MPI_THREAD_SERIALIZED, &provided );
	 * MPI_Init_thread( 0, 0, MPI_THREAD_MULTIPLE, &provided );
	 *                 
	 ***/ 

	MPI_Init_thread(0, 0, MPI_THREAD_MULTIPLE, &provided );
	MPI_Query_thread( &claimed );
	std::cout << "Query thread level = " << claimed << " - provided = " << provided << std::endl;
						  
	MPI_Finalize();


	return 0;
}
