#include <cstdlib>
#include <math.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <mpi.h>


#define SEC_TO_MS(x) (1000*(x))

#define GENERAL_PROCCESS 0
#define ARR_LENGTH 8

int main(int argc, char** argv) {
    srand(time(0));

    int ProcNum;
    int ProcRank;

    int arrLength = ARR_LENGTH;
    int message_buffer_size = arrLength * sizeof(int) + MPI_BSEND_OVERHEAD;

    int *sbuf = new int[arrLength];
    int *rbuf = new int[arrLength];
    int *buf = (int*) malloc(message_buffer_size);
    double startTime, endTime;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

    if (ProcRank == GENERAL_PROCCESS) {
        for (int i = 0; i < arrLength; ++i) {
            sbuf[i] = rand() % 10000;
        }
    }

    if (ProcRank == GENERAL_PROCCESS) {
        printf("Array Length: %d\n", arrLength);
        startTime = MPI_Wtime();
        MPI_Send(sbuf, arrLength, MPI_INT, 1, 2, MPI_COMM_WORLD);
        MPI_Recv(rbuf, arrLength, MPI_INT, 1, 2, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
        endTime = MPI_Wtime();
        printf("Default Send - Recv\nTime Send: %.3f \n", SEC_TO_MS(endTime - startTime));


        // Synchronous send
        startTime = MPI_Wtime();
        MPI_Ssend(sbuf, arrLength, MPI_INT, 1, 3, MPI_COMM_WORLD);
        MPI_Recv(rbuf, arrLength, MPI_INT, 1, 3, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
        endTime = MPI_Wtime();
        printf("Synchronous send\nTime Ssend: %.3f \n", SEC_TO_MS(endTime - startTime));
        MPI_Barrier(MPI_COMM_WORLD);

        // Buffered send
        startTime = MPI_Wtime();
        // attach to buffer for buffered messages
        MPI_Buffer_attach(buf, message_buffer_size);
        MPI_Bsend(sbuf, arrLength, MPI_INT, 1, 4, MPI_COMM_WORLD);
        MPI_Recv(rbuf, arrLength, MPI_INT, 1, 4, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
        endTime = MPI_Wtime();
        printf("Buffered send\nTime Bsend: %.3f \n", SEC_TO_MS(endTime - startTime));
        MPI_Buffer_detach(buf, &message_buffer_size);
        free(buf);

        // Ready send
        startTime = MPI_Wtime();
        MPI_Rsend(sbuf, arrLength, MPI_INT, 1, 5, MPI_COMM_WORLD);
        MPI_Recv(rbuf, arrLength, MPI_INT, 1, 5, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
        endTime = MPI_Wtime();
        printf("Ready send\nTime Rsend: %.3f \n", SEC_TO_MS(endTime - startTime));


    } else {
        MPI_Recv(rbuf, arrLength, MPI_INT, 0, 2, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
        MPI_Send(sbuf, arrLength, MPI_INT, 0, 2, MPI_COMM_WORLD);

        MPI_Recv(rbuf, arrLength, MPI_INT, 0, 3, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
        MPI_Ssend(sbuf, arrLength, MPI_INT, 0, 3, MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD);

        MPI_Recv(rbuf, arrLength, MPI_INT, 0, 4, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
        MPI_Buffer_attach(buf, message_buffer_size);
        MPI_Bsend(sbuf, arrLength, MPI_INT, 0, 4, MPI_COMM_WORLD);
        MPI_Buffer_detach(buf, &message_buffer_size);
        free(buf);

        MPI_Recv(rbuf, arrLength, MPI_INT, 0, 5, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
        MPI_Rsend(sbuf, arrLength, MPI_INT, 0, 5, MPI_COMM_WORLD);
    }

    MPI_Finalize();
}