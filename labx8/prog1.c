/* Program for Pi calculating using MPI.
 *
 * How to compile:
 * module load mpi/openmpi-3.1.2
 * mpicc -o pim -O3 piprog_MPI.c -lm
 * mpirun -np 4 -mca btl_base_warn_component_unused 0 ./pim
 *
 *
 * Results of runnning with different nProc:
 *------------------------------------------------
 *|  nProc  |   Trials   |   Elapsed   |   dpi   |
 *|----------------------------------------------|	
 *|    4    |   32*10^9  |   336.96    |  2e-05  |
 *|----------------------------------------------|
 *|    3    |   24*10^9  |   155.85    |  3e-05  |
 *|----------------------------------------------|
 *|    2    |   16*10^9  |   120.72    |  4e-05  |
 *|----------------------------------------------|
 *|    1    |    8*10^9  |   123.29    |  5e-05  |
 *|----------------------------------------------|
 *
 * 03.12.21. Alexander Blagodarnyi
 *
 * */

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

#define NPOINTS     8000000000
#define GENSEED     111111

#define NANOSEC_in_SEC 1000000000LL
#define dsecond(tx,ty) ((double)((ty.tv_sec)-(tx.tv_sec))+(double)((ty.tv_nsec)-(tx.tv_nsec))/NANOSEC_in_SEC)


int main(int argc, char *argv[]){
    long ncirc = 0;
    double pi, dpi;
    int numthrd;
    int mythrid;

    unsigned long long num_trials = NPOINTS;
    int rank, size;
    struct timespec tstart, tend;

    MPI_Init(&argc , &argv);			
    MPI_Comm_size(MPI_COMM_WORLD, &numthrd);
    MPI_Comm_rank(MPI_COMM_WORLD, &mythrid);

    clock_gettime(CLOCK_REALTIME, &tstart);

{
    double x,y,t,dres1, dres2;
    struct drand48_data rbuf;
    long rseed = (mythrid+1)*GENSEED;
    unsigned long long local_ncirc = 0;
    unsigned long long i;

    srand48_r(rseed,&rbuf);

    for (i =0; i< num_trials; i++ ){
        drand48_r(&rbuf, &dres1);
        drand48_r(&rbuf, &dres2);

        x = 2.0 * dres1 -1;
        y = 2.0 * dres2 -1;
        t = x*x + y*y;

        if (t <= 1.0){
            local_ncirc++;
        }

    }
    MPI_Reduce(&local_ncirc, &ncirc,1 , MPI_LONG_LONG_INT, MPI_SUM, 0, MPI_COMM_WORLD);
}

clock_gettime(CLOCK_REALTIME, &tend);
double tlaps = dsecond(tstart, tend);

num_trials *= numthrd;
pi = 4.0 * (double) ncirc /(double) num_trials;
dpi = pi*sqrt(2.0/num_trials);
if(mythrid==0)
fprintf(stdout, "Trials: %ld Ncirc: %ld Threads: %d Elapsed: %.2f PI: %.8lf dpi: %.lg\n", num_trials, ncirc, numthrd, tlaps, pi, dpi);
MPI_Finalize();
return 0;

}

