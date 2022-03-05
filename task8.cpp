#include <cstdlib>
#include <math.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <mpi.h>

#define GENERAL_PROCCESS 0
#define ARR_LENGTH 8

int main(int argc, char** argv) {
    srand(time(0));

    int ProcNum;
    int ProcRank;
    int arrLength = ARR_LENGTH;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    int partition = ceil ((double)arrLength / ProcNum);
    printf("partition %d\n", partition);
    int *arr = new int[arrLength];
    int *buf = new int[partition];
    int *result_array = new int[arrLength];

    if (ProcRank == GENERAL_PROCCESS) {

        for (int i = 0; i < arrLength; ++i) {
            arr[i] = rand() % 1000;
            printf("%d ", arr[i]);
        }
        printf("\n\n");

        // from GENERAL_PROCCESS send to other processes
        for (int i = 0; i < ProcNum; ++i) {
            int *send_buf = new int[partition];
            for (int j = 0; j < partition; ++j) {
                send_buf[j] = arr[i * partition + j];
            }
            if (i == 0) {
                buf = send_buf;
            } else {
                MPI_Send(send_buf, partition, MPI_INT, i, i + 111, MPI_COMM_WORLD);
            }
        }
    }

    // on other processes get data
    if (ProcRank != GENERAL_PROCCESS) {
        MPI_Recv(buf, partition, MPI_INT, GENERAL_PROCCESS, ProcRank + 111, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
    }
    
    // print thread part of data
    printf("thread (rank) = %d\n", ProcRank);
    for (int i = 0; i < partition && i < arrLength-(partition * ProcRank); ++i) {
        printf("%d ", buf[i]);
    }
    printf("\n\n");
    fflush(stdout);

    // DO SOME WORK ..

    if (ProcRank != GENERAL_PROCCESS) {
        // from other processes sent to GENERAL_PROCCESS himself part
        MPI_Send(buf, partition, MPI_INT, 0, ProcRank + 222, MPI_COMM_WORLD);
    } else {
        // if GENERAL_PROCCESS, fill last part of result array from buf in GENERAL_PROCCESS
        for (int i = 0; i < partition; ++i) {
            result_array[i] = buf[i];
        }
    }

    if (ProcRank == GENERAL_PROCCESS) {
        // get data in GENERAL_PROCCESS and fill result array from buf 
        for (int i = 1; i < ProcNum; ++i) {
            MPI_Recv(buf, partition, MPI_INT, i, i + 222, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
            for (int j = 0; j < partition; ++j) {
                result_array[i*partition+j] = buf[j];
            }
        }

        
        printf("Answer: ");
        for (int i = 0; i < arrLength; ++i) {
            printf("%d ", result_array[i]);
        }
    }

    MPI_Finalize();
}