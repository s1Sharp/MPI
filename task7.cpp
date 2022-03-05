#include <cstdlib>
#include <math.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <mpi.h>

#define GENERAL_PROCCESS 0
#define LINES 4
#define COLUMNS 4


#define RANDOM_SPREAD_COEFFICIENT 2
template<typename T>
void rand_vec(T* vec, size_t size)
{
    int radius = ((int)size * RANDOM_SPREAD_COEFFICIENT);
	for (size_t i = 0; i < size; ++i)
	{
		vec[i] = rand() % radius + 1;
	}
    return;
}

int main(int argc, char** argv) {
    srand(time(0));
    int lines = LINES, columns = COLUMNS;

    int ProcNum;
    int ProcRank;
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

    if (ProcNum > LINES || ProcNum > COLUMNS) { MPI_Finalize(); printf("Invalid Matrix dimention > ProcNum\n"); return 0;};

    //init memory
    int **arr;
    arr = new int *[lines];
    int *vector = new int[columns];

    int partition = (int) round((double) columns / ProcNum);

    if (columns % ProcNum != 0) {
        if (ProcRank == ProcNum - 1) partition = columns - (ProcNum - 1) * partition;
    }
    int *send_arr = new int[columns * lines];

    int *buf_arr = new int[partition * lines];
    int *buf_vector = new int[partition];


    int *displs = new int[ProcNum];
    int *sendcounts_mat = new int[ProcNum];

    int *displs_vec = new int[ProcNum];
    int *sendcounts_vec = new int[ProcNum];

    // fill matrix and vector
    if (ProcRank == GENERAL_PROCCESS) {
        //fill matrix
        for (int i = 0; i < lines; i++) {
            arr[i] = new int[columns];
            rand_vec(arr[i], columns);
            for (int j = 0; j < columns; ++j) {
                send_arr[lines * j + i] = arr[i][j];
            }
        }
        //fill vector
        rand_vec(vector, columns);

        //compute size of chunks
        for (int i = 0; i < ProcNum; i++) {
            displs[i] = i * partition * lines;
            sendcounts_mat[i] = partition * lines;
            displs_vec[i] = i * partition;
            sendcounts_vec[i] = partition;
        }

        if (columns % ProcNum != GENERAL_PROCCESS) {
            sendcounts_mat[ProcNum - 1] = (columns - (ProcNum - 1) * partition) * lines;
            sendcounts_vec[ProcNum - 1] = columns - (ProcNum - 1) * partition;
        }

    }

    // send matrix column chunks to each thread
    MPI_Scatterv(send_arr, sendcounts_mat, displs, MPI_INT, buf_arr, partition * lines, MPI_INT, 0, MPI_COMM_WORLD);
    // send vector chunks to each thread
    MPI_Scatterv(vector, sendcounts_vec, displs_vec, MPI_INT, buf_vector, partition, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);


    int *part_sum_vec = new int[lines];
    int *res_sum_vec = new int[lines];
    for (int i = 0; i < lines; ++i) {
        part_sum_vec[i] = 0;
    }

    // comppute part of vector result
    for (int i = 0; i < lines; ++i) {
        for (int j = 0; j < partition; ++j) {
            part_sum_vec[i] += buf_vector[j] * buf_arr[lines * j + i];
        }
    }

    // reduce local sum chunk vectors into result vector
    MPI_Reduce(part_sum_vec, res_sum_vec, lines, MPI_INT, MPI_SUM, GENERAL_PROCCESS, MPI_COMM_WORLD);

    if (ProcRank == GENERAL_PROCCESS) {
        printf("\nmatrix\n")ж
        for (int i = 0; i < lines; i++) {
            for (int j = 0; j < columns; ++j) {
                printf("%d ", arr[i][j]);
            }
            printf("\n");
        }

        printf("\nvector\n");
        for (int j = 0; j < columns; ++j) {
            printf("%d ", vector[j]);
        }

        printf("\n\nresult vector\n");
        for (int i = 0; i < lines; ++i) {
            printf("%d ", res_sum_vec[i]);
        }

    }

    MPI_Finalize();
    return 0;
}