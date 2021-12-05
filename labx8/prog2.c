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
 *|    4    |   32*10^9  |   124.44    |  2e-05  |
 *|----------------------------------------------|
 *|    3    |   24*10^9  |   124.42    |  3e-05  |
 *|----------------------------------------------|
 *|    2    |   16*10^9  |   124.27    |  4e-05  |
 *|----------------------------------------------|
 *|    1    |    8*10^9  |   124.18    |  5e-05  |
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
#include <omp.h>

#define NPOINTS     8000000000
#define GENSEED     111111

#define NANOSEC_in_SEC 1000000000LL
#define dsecond(tx,ty) ((double)((ty.tv_sec)-(tx.tv_sec))+(double)((ty.tv_nsec)-(tx.tv_nsec))/NANOSEC_in_SEC)


int main(int argc, char *argv[]){
    long ncirc = 0;
    double pi, dpi;

    unsigned long long num_trials = NPOINTS;
    int rank, size;
    struct timespec tstart, tend;

    MPI_Init(&argc , &argv);	          // Start MPI 		
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Initialize number of procs
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // nitialize current proc

    clock_gettime(CLOCK_REALTIME, &tstart);  

    unsigned long long mpi_ncirc = 0;      // ncirc for each MPI proc
#pragma omp parallel default(none) firstprivate(num_trials) shared(ncirc) // do in parallel
{
    double x,y,t,dres1, dres2;
    struct drand48_data rbuf;
    int thrd_num = omp_get_thread_num();
    long rseed = (rank+1+thrd_num)*GENSEED; // seed depend on rank and thread number
    unsigned long long local_ncirc = 0;     // local ncirc for each thread
    unsigned long long i;

    srand48_r(rseed,&rbuf);
    
    #pragma omp for //reduction(+:local_ncirc) // split the work
    for (i =0; i < num_trials; i++ ){
        drand48_r(&rbuf, &dres1);
        drand48_r(&rbuf, &dres2);

        x = 2.0 * dres1 -1;
        y = 2.0 * dres2 -1;
        t = x*x + y*y;

        if (t <= 1.0){
            local_ncirc++; 
        }
    }
    #pragma omp atomic
    mpi_ncirc += local_ncirc // end parallel region. add result to mpi result
   MPI_Reduce(&mpi_ncirc, &ncirc,1 , MPI_LONG_LONG_INT, MPI_SUM, 0, MPI_COMM_WORLD); // finish process
}
clock_gettime(CLOCK_REALTIME, &tend);
double tlaps = dsecond(tstart, tend);

num_trials *= size;
pi = 4.0 * (double) ncirc /(double) num_trials;
dpi = pi*sqrt(2.0/num_trials);
if(rank==0)
fprintf(stdout, "Trials: %ld Ncirc: %ld Threads: %d Elapsed: %.2f PI: %.8lf dpi: %.lg\n", num_trials, ncirc, size, tlaps, pi, dpi);
MPI_Finalize();  // finish MPI
return 0;

}

