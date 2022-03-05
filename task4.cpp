#include "stdio.h"
#include "mpi.h"
#include "stdlib.h"
#include "math.h"
#include <iostream>
#include <ctime>


#define MAS_SIZE 10000
#define GENERAL_PROCCESS 0
#define ANY_TAG 0

#define RANDOM_SPREAD_COEFFICIENT 3
template<typename T>
void rand_vec(T* vec, size_t size)
{
	for (size_t i = 0; i < size; ++i)
	{
        int radius = ((int)size * RANDOM_SPREAD_COEFFICIENT);
		vec[i] = rand() % radius - radius/2;
	}
    return;
}


int main(int argc, char** argv)
{
	srand(time(0));
    int ProcNum;
    int ProcRank;

    int root = GENERAL_PROCCESS;
    int data[MAS_SIZE];
    int data_loc[MAS_SIZE];
    float final_res[MAS_SIZE];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

    int mas_size = MAS_SIZE;
    if (ProcRank == GENERAL_PROCCESS) {
        rand_vec(data, mas_size);
    }
    //broad cast data size
    MPI_Bcast(&mas_size, 1, MPI_INT, root, MPI_COMM_WORLD);

    int chunk_size = mas_size / ProcNum;

    //send to all threads data portion sizeof(chunk_size)
    MPI_Scatter(&data, chunk_size, MPI_INT, 
                data_loc, chunk_size, MPI_INT, root, MPI_COMM_WORLD);

    int local_sum = 0;
    for(int i=0; i < chunk_size; i++) {
        if (data_loc[i] >= 0) {
            local_sum += data_loc[i];   
        }  
    }

    float local_avg = (float) local_sum / (float) chunk_size;
    printf("Local average: %f, thread: %d\n", local_avg, ProcRank);
    //fix proccess stdout not right time
    //explicitly clear standard output
    fflush(stdout);
    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Gather(&local_avg, 1, MPI_FLOAT, final_res, 1, MPI_FLOAT, root, MPI_COMM_WORLD);
    //data reductuion
    if (ProcRank == GENERAL_PROCCESS) {
        float global_avg = 0;
        //MPI_Reduce(&local_avg, &global_avg, 1, MPI_FLOAT, MPI_SUM, GENERAL_PROCCESS, MPI_COMM_WORLD);

        for(int i=0; i < ProcNum; i++)
            global_avg += final_res[i];
        global_avg = global_avg / (float) ProcNum;
 
        printf("Final average: %f \n", global_avg);
    }

    MPI_Finalize();
    return 0;
}