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
    
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    if (ProcNum > LINES || ProcNum > COLUMNS) { MPI_Finalize(); printf("Invalid Matrix dimention > ProcNum\n"); return 0;};

    int **arr;
    int max_of_min;
    arr = new int *[lines];

    int partition = (int) round((double) lines / ProcNum);

    if(lines % ProcNum != 0) {
        if (ProcRank == ProcNum - 1) partition = lines - partition * (ProcNum-1);
    }

    int *displs = new int[ProcNum];
    int *sendcounts = new int[ProcNum];
    int *send_arr = new int[lines * columns];

    int *buf = new int[partition * columns];
// генерируем матрицу, и представляем ее в виде массива
    if (ProcRank == GENERAL_PROCCESS) {

        for (int i = 0; i < lines; i++) {
            arr[i] = new int[columns];
            rand_vec(arr[i], columns);
            for (int j = 0; j < columns; j++) {
                printf("%d ", arr[i][j]);
                send_arr[columns * i + j] = arr[i][j];
            }
            printf("\n");
        }

        for (int i = 0; i < ProcNum; i++) {
            displs[i] = i * partition * columns;
            sendcounts[i] = partition * columns;
        }

        if(lines % ProcNum != 0) {
            sendcounts[ProcNum-1] = (lines - partition * (ProcNum-1)) * columns;
        }
    }

// послыаем каждому процессу часть массива
    MPI_Scatterv(send_arr, sendcounts, displs, MPI_INT, buf, partition * columns, MPI_INT, GENERAL_PROCCESS, MPI_COMM_WORLD);

    int local_ans_max = INT_MIN;

    // проходим по каждой строке
    for (int i = 0; i < partition; ++i) {
        int local_min = buf[columns * i];
        // проходим по каждому элементу в строке; ищем в каждой строке минимум
        for (int j = 0; j < columns; ++j) {
            // ищем в каждой строке минимум
            if (buf[columns * i + j] < local_min) local_min = buf[columns * i + j];
        }
        // максимум среди минимумов
        if (local_ans_max < local_min) local_ans_max = local_min;
    }

    MPI_Reduce(&local_ans_max, &max_of_min, 1, MPI_INT, MPI_MAX, GENERAL_PROCCESS, MPI_COMM_WORLD);

    if (ProcRank == GENERAL_PROCCESS) {
        printf("Max of min: %d \n",max_of_min);
    }
    
    MPI_Finalize();
    return 0;
}