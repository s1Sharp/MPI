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
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

    int partition =(int) round( (double)arrLength / ProcNum);

    if(arrLength % ProcNum != GENERAL_PROCCESS) {
        if (ProcRank == ProcNum - 1) partition = arrLength - partition * (ProcNum-1);
    }

    int *arr = new int[arrLength];
    int *reversed_array = new int[arrLength];
    int *displs = new int[ProcNum];

    // сдвиг при приеме на 0 процессе
    int *recv_displs = new int[ProcNum];
    int *sendcounts = new int[ProcNum];

    int *buf = new int[partition];

    if (ProcRank == GENERAL_PROCCESS) {
        printf("Array: \n");
        for (int i = 0; i < arrLength; ++i) {
            arr[i] = rand() % 1000;
            printf("%d ", arr[i]);
        }
        printf("\n");

        for (int i = 0; i < ProcNum; ++i) {
            displs[i] = i * partition;
            sendcounts[i] = partition;
        }

        if (arrLength % ProcNum != GENERAL_PROCCESS) {
            sendcounts[ProcNum-1] = arrLength - partition * (ProcNum-1);
        }
        recv_displs[ProcNum-1] = 0;

        for (int i = ProcNum - 2; i >= 0; i--) {
            recv_displs[i] = recv_displs[i+1] + sendcounts[i+1];
        }
    }

    // send chunk of array from GENERAL_PROCCESS to each other processes
    MPI_Scatterv(arr, sendcounts, displs, MPI_INT, buf, partition, MPI_INT, GENERAL_PROCCESS, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

    // each process revert himself part of data
    for (int i = 0; i < partition / 2; ++i) {
        std::swap(buf[i], buf[partition - i - 1]);
    }

    // each process send to GENERAL_PROCCESS reverted data
    MPI_Gatherv(buf, partition, MPI_INT, reversed_array, sendcounts, recv_displs, MPI_INT, GENERAL_PROCCESS, MPI_COMM_WORLD);

    if (ProcRank == GENERAL_PROCCESS) {
        printf("Reversed Array: \n");
        for (int i = 0; i < arrLength; ++i) {
            printf("%d ", reversed_array[i]);
        }
    }

    MPI_Finalize();
    return 0;
}
