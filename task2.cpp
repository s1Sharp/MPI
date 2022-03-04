#include "stdio.h"
#include "mpi.h"
#include "stdlib.h"
#include "math.h"
#include <iostream>
#include <ctime>


#define MAS_SIZE 100
#define GENERAL_PROCCESS 0
#define ANY_TAG 0
#define RANDOM_SPREAD_COEFFICIENT 3

template<typename T>
void rand_vec(T* vec, size_t size)
{
	for (size_t i = 0; i < size; ++i)
	{
		vec[i] = rand() % ((int)size * RANDOM_SPREAD_COEFFICIENT) + 1;
	}
    return;
}

int main(int argc, char *argv[]){   

    int TotalMax = INT_MIN, LocalMax = INT_MIN;
	srand(time(0));
    int a[MAS_SIZE];


    int ProcNum;
    int ProcRank;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    if (ProcRank == GENERAL_PROCCESS)
    {
	    rand_vec(a, MAS_SIZE);
    }

    //broadcast data to all other process
    MPI_Bcast(&a, MAS_SIZE, MPI_INT, GENERAL_PROCCESS, MPI_COMM_WORLD);

    //find local LocalMax
    //find in range [idx1 ; idx2]
    int k = MAS_SIZE / ProcNum;
    int idx1 = k * ProcRank;
    int idx2 = k * (ProcRank + 1);

    if (ProcRank == ProcNum - 1) idx2 = MAS_SIZE; //end of range

    for(size_t i = idx1; i < idx2; i++)
    {
        if (LocalMax < a[i]) LocalMax = a[i];
    }

    if (ProcRank == GENERAL_PROCCESS)
    {
        TotalMax = LocalMax;
        for (size_t i = GENERAL_PROCCESS + 1; i < ProcNum; i++) {
            MPI_Recv(&LocalMax, 1, MPI_INT, MPI_ANY_SOURCE, ANY_TAG, MPI_COMM_WORLD, &status);

            printf("\nTotal Max = %d, Local Max = %d from %d procces", TotalMax, LocalMax, status.MPI_SOURCE);

            if (TotalMax < LocalMax) TotalMax = LocalMax;
        }
    }
    else //send msg from another process about LocalMax
    {
        MPI_Send(&LocalMax, 1, MPI_INT, GENERAL_PROCCESS, ANY_TAG, MPI_COMM_WORLD);
    }

    //print result
    if (ProcRank == GENERAL_PROCCESS){
        printf("\nTotal Max = %d\n", TotalMax);
    }

    MPI_Finalize();
}
