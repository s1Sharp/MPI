#include <cstdlib>
#include <math.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <mpi.h>

#define STDOUT_FLUSHING

#define GENERAL_PROCCESS 0
#ifdef STDOUT_FLUSHING
#define flushing fflush(stdout);
#else 
#define flushing ;
#endif

#define MATRIX_SIZE 10


int main(int argc, char **argv) {
    int matrix_size = MATRIX_SIZE;
    int procRank, procNum;
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    MPI_Comm_size(MPI_COMM_WORLD, &procNum);

    int **arr = new int *[matrix_size];
    int length_send_arr = matrix_size * matrix_size - matrix_size;
    int *send_arr = new int[length_send_arr];

    int partition = (int) round((double) length_send_arr / (procNum * 2));

    // set portion for lasts threads
    if (length_send_arr % (procNum * 2) != 0) {
        if (procRank == procNum - 1) partition = (length_send_arr / 2) - (procNum - 1) * partition;
    }

    int *buf_arr = new int[partition * 2];
    int *displs = new int[procNum];
    int *sendcounts = new int[procNum];

    // count the same a[ij] a[ji] elems
    int count = 0;

    if (procRank == GENERAL_PROCCESS) {
        for (int i = 0; i < matrix_size; ++i) {
            arr[i] = new int[matrix_size];
            for (int j = 0; j < matrix_size; ++j) {
                arr[i][j] = i + j;
                printf("%d\t", arr[i][j]);
            }
            printf("\n");
        }
        printf("\n");

        int index = 0;

        // . . . . . . . . 
        //   . . . . . . . 
        //     . . . . . .
        //       . . . . .
        //         . . . . 
        //           . . .
        //             . .
        //               .
        // collapse matrix, in array with elems [ij] [ji] to array [index] [index+1]
        for (int i = 1; i < matrix_size; ++i) {
            for (int j = 0; j < i; ++j) {
                send_arr[index] = arr[i][j];
                send_arr[index + 1] = arr[j][i];
                index += 2;
            }
        }

        for (int i = 0; i < procNum; ++i) {
            displs[i] = i * partition * 2;
            sendcounts[i] = partition * 2;
        }
        if (length_send_arr % (procNum * 2) != 0) {
            sendcounts[procNum-1] = ((length_send_arr / 2) - (procNum - 1) * partition) * 2;
        }
    }

    MPI_Scatterv(send_arr, sendcounts, displs, MPI_INT, buf_arr, partition*2, MPI_INT, GENERAL_PROCCESS, MPI_COMM_WORLD);

    int local_count = 0;

    // for each thread, fill local count the same elems
    for (int i = 0; i < partition*2 - 1; i += 2) {
        printf("procRank: %d, %d %d\n", procRank, buf_arr[i], buf_arr[i+1]);
        if (buf_arr[i] == buf_arr[i + 1]) {
            local_count++;
        }
    }
    printf("\n");

    MPI_Reduce(&local_count, &count, 1, MPI_INT, MPI_SUM, GENERAL_PROCCESS, MPI_COMM_WORLD);

    if (procRank == GENERAL_PROCCESS) {
        if (count == length_send_arr / 2) {
            printf("Matrix is symmetric");
        } else {
            printf("Matrix not symmetric");
        }
    }

    MPI_Finalize();
    return 0;
}