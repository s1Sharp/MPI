#include <cstdlib>
#include <math.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <mpi.h>

using namespace std;

#define SEC_TO_MS(x) (1000*(x))

#define GENERAL_PROCCESS 0
#define RANDOM_SPREAD_COEFFICIENT 3

void RandomDataGeneration(double* pVector, int Size)
{
    int radius = ((int)Size * RANDOM_SPREAD_COEFFICIENT);
	for (int i = 0; i < Size; i++) {
		pVector[i] = rand() % radius - radius/2;
	}
}

double Multiplication(double *vector1, double* vector2, int size)
{
	double result = 0;
	for (int i = 0; i < size; i++) {
		result += vector1[i] * vector2[i];
	}
	return result;
}


int main(int argc, char** argv)
{
    int ProcRank = 0;
    int ProcNum = 0;
	double* pVector1; // Первый вектор для умножения
	double* pVector2; // Второй вектор для умножения
	double* procVector1; //Распараллеленный кусок первого вектора
	double* procVector2; //Распараллеленный кусок второго вектора
	double pResult = 0.0; // Результат скалярного умножения векторов
	double currentResult; //Результат умножения "маленьких" векторов
	int Size; // Размеры исходных векторов
	int currentSize; // Размер распараллеленных векторов
	double startTime, endTime;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
	MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
	
	Size = atoi(argv[1]);

	if (ProcRank == GENERAL_PROCCESS) {
		pVector1 = new double [Size];
		pVector2 = new double [Size];

		RandomDataGeneration(pVector1, Size);
		RandomDataGeneration(pVector2, Size);
	}

	currentSize = Size / ProcNum;
	//printf("curr size: %d\n", currentSize);

	procVector1 = new double[ currentSize ];
	procVector2 = new double[ currentSize ];

	startTime = MPI_Wtime();

	MPI_Scatter(pVector1, currentSize, MPI_DOUBLE,
		procVector1, currentSize, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Scatter(pVector2, currentSize, MPI_DOUBLE,
		procVector2, currentSize, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	currentResult = Multiplication(procVector1, procVector2, currentSize);
	printf("local result:%f, thread %d\n", currentResult, ProcRank);
    //fix proccess stdout not right time
    //explicitly clear standard output
    fflush(stdout);
    MPI_Barrier(MPI_COMM_WORLD);

	MPI_Reduce(&currentResult, &pResult, 1,
		MPI_DOUBLE, MPI_SUM, GENERAL_PROCCESS, MPI_COMM_WORLD);

	endTime = MPI_Wtime();

	if (ProcRank == GENERAL_PROCCESS) {
        printf("calculation result: %f\n", pResult);
		printf("Size: %d ProcNum: %d time ms: %f Precision: %lf\n", Size, 
			ProcNum, SEC_TO_MS(endTime - startTime), MPI_Wtick());
	}

	MPI_Finalize();
	return 0;
}