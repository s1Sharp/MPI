#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include <time.h>
 
#define SEC_TO_MS(x) (1000*(x))
#define GENERAL_PROCCESS 0

int main(int argc, char* argv[]) {

    int ProcNum;
    int ProcRank;

    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    
    if(argc !=2 || atoi(argv[1]) > (INT_MAX / 4)) {
        if (ProcRank == GENERAL_PROCCESS) {
            fprintf(stderr,"Incorrect Iter Count\n");
            fflush(stderr);
        }
        MPI_Abort(MPI_COMM_WORLD,1);
    }

    int i,IterCount;
    double x, y;
    double IterCountInput;
    double endTime,startTime,elapsedTime;
    int local_hit;
    
    sscanf_s(argv[1], "%lf", &IterCountInput);
    IterCount = lround(IterCountInput);
    MPI_Barrier(MPI_COMM_WORLD);

    startTime = MPI_Wtime();
    local_hit = 0;
    srand((unsigned)(time(0)));
    int local_IterCount = IterCount / ProcNum;
    
    for(i = 0; i < local_IterCount; i++) {
        x = ((double)rand())/((double)RAND_MAX);
        y = ((double)rand())/((double)RAND_MAX);
        if (((x*x) + (y*y)) <= 1) local_hit++;
    }
    
    int global_hit = 0;
    MPI_Allreduce(&local_hit,&global_hit,1,MPI_INT,MPI_SUM,MPI_COMM_WORLD);
    double pi;
    pi = (global_hit * 4) / ((double)IterCount);
    MPI_Barrier(MPI_COMM_WORLD);
    
    endTime = MPI_Wtime();
    elapsedTime = fabs(endTime - startTime);
    
    if (ProcRank == GENERAL_PROCCESS) {
        printf("Number of Points Used:\t%d\n", IterCount);
        printf("Estimate of Pi:\t%24.16f\n", pi);
        printf("Elapsed Wall time ms:\t%.5f\n", SEC_TO_MS(elapsedTime));
    }
    
    MPI_Finalize();
    return 0;
}