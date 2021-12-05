/*
 * Program calculates PI number. The number of trials is proportional to the number of threads.
 * 
 * How to compile:
 *  gcc -o piprog3 -fopenmp -O3 piprog_G.c -lm
 *
 *
 * Results :
 * |-------------------------------------------------------------|
 * |    #threads     |      1       |       2      |      3      |
 * |-------------------------------------------------------------| 
 * |   Elapsed time  |    299.86    |    300.36    |    301.96   |
 * |-------------------------------------------------------------|
 * |     value Pi    |   3.1416034  |  3.14159559  |  3.1459153  |
 * |-------------------------------------------------------------|
 * |    Error in pi  |     3e-05    |     2e-05    |    1e-05    |
 * |-------------------------------------------------------------|
 *
 * 03.12.21. Alexander Blagodarnyi
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define NPOINTS		26000000000	// number trials
#define GENSEED		1235791		// prime random seed

int main()
{
    long ncirc = 0;
    double pi, dpi;
    int size = omp_get_max_threads();
    unsigned long long num_trials = NPOINTS;

    double tstart = omp_get_wtime();

#pragma omp parallel default(none) firstprivate(num_trials) shared(ncirc)
{
    double x, y, t, dres1, dres2;
    struct drand48_data rbuf;
    int rank = omp_get_thread_num();
    long rseed = (rank+1) * GENSEED;
    unsigned long long local_ncirc = 0;
    unsigned long long i;

    srand48_r(rseed, &rbuf);

    for( i = 0; i < num_trials; i++ ) {

	drand48_r(&rbuf, &dres1);      // re-entrant random num gen
	drand48_r(&rbuf, &dres2);      // [0..1)

	x = 2.0 * dres1 - 1;	       // place the circle around {0,0}
	y = 2.0 * dres2 - 1;
	t = x*x + y*y;

	if( t <= 1.0 ) local_ncirc++;
    }
    #pragma omp atomic
	ncirc += local_ncirc;
} // end parallel region

    double tend = omp_get_wtime();
    double tlaps = tend - tstart;

    num_trials *= size; // all threads the same work
    pi = 4.0 * (double) ncirc/ (double) num_trials;
    dpi = pi*sqrt(2.0/num_trials);
    fprintf(stdout,"Trials: %ld Ncirc: %ld Threads: %d Elapsed: %.2f  PI: %.8lf dpi: %.1g\n",
		    num_trials, ncirc, size, tlaps, pi, dpi);

    return 0;
}
