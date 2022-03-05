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

#define NUMBER 10

void circle(int, MPI_Comm);

int main(int argc, char **argv) {
    int num = NUMBER;
    int procNum, procRank;

    MPI_Init(&argc, &argv);

    MPI_Group MPI_GROUP_WORLD, NEW_GROUP_NAME;
    MPI_Comm NEW_COMM;

    MPI_Comm_size(MPI_COMM_WORLD, &procNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    MPI_Comm_group(MPI_COMM_WORLD, &MPI_GROUP_WORLD);
    int new_group_count = procNum / 2;

    // дet's take the ranks for new group in 1 (step ahead 2)
    int *ranks = new int[new_group_count];
    for (int i = 0; i < new_group_count; ++i) {
        ranks[i] = i * 2;
    }

    // create new group for mainly commutator MPI_GROUP_WORLD
    MPI_Group_incl(MPI_GROUP_WORLD, new_group_count, ranks, &NEW_GROUP_NAME);

    // create new commutator for new created group
    MPI_Comm_create(MPI_COMM_WORLD, NEW_GROUP_NAME, &NEW_COMM);

    if (NEW_COMM != MPI_COMM_NULL) { //check if commutator was created and NOT NULL
        circle(num, NEW_COMM);
    }

    if (procRank == GENERAL_PROCCESS) {
    MPI_Group_free(&MPI_GROUP_WORLD);
    MPI_Group_free(&NEW_GROUP_NAME);
    MPI_Comm_free(&NEW_COMM);
    }

    MPI_Finalize();


    return 0;
}

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