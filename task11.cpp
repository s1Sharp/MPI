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

void circle(int num, MPI_Comm comm) {
    int procRank, procNum;

    MPI_Comm_rank(comm, &procRank);
    MPI_Comm_size(comm, &procNum);

    int *send = new int;
    int *recv = new int;

    int to_proc_id = procRank + 1;
    int from_proc_id = procRank - 1;

    // create circle
    if (procRank == GENERAL_PROCCESS) {
        from_proc_id = procNum - 1;
    }
    if (procRank == procNum - 1) {
        to_proc_id = GENERAL_PROCCESS;
    }

    // GENERAL_PROCCESS send 'num' to next process
    if (procRank == GENERAL_PROCCESS) {
        *send = num;
        MPI_Send(send, 1, MPI_INT, to_proc_id, to_proc_id, comm);
        printf("Send from_proc_id %d to_proc_id %d: %d\n", procRank, to_proc_id, *send);
        flushing
    }
    if (procRank != GENERAL_PROCCESS) {
        // each process get an number, add 5 and send next
        MPI_Recv(recv, 1, MPI_INT, from_proc_id, procRank, comm, MPI_STATUSES_IGNORE);
        printf("Receive from_proc_id %d to_proc_id %d: %d\n", from_proc_id, procRank, *recv);
        flushing
        *send = (*recv) + 5;

        MPI_Send(send, 1, MPI_INT, to_proc_id, to_proc_id, comm);
        printf("Send from_proc_id %d to_proc_id %d: %d\n", procRank, to_proc_id, *send);
        flushing
    }

    if (procRank == GENERAL_PROCCESS) {
        // GENERAL_PROCCESS get number from last process (procNum - 1) 
        MPI_Recv(recv, 1, MPI_INT, from_proc_id, 0, comm, MPI_STATUSES_IGNORE);
        printf("Receive from_proc_id %d to_proc_id %d\n", from_proc_id, procRank);
        flushing
        printf("Result: %d\n", *recv);
        flushing
    }
}

#define NUMBER 10

int main(int argc, char **argv) {
    int num = NUMBER;
    MPI_Init(&argc, &argv);

    circle(num, MPI_COMM_WORLD);

    MPI_Finalize();
    return 0;
}