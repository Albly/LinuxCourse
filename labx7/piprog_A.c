/* This program calculates PI number. Number of trials is a constant.
 * 
 * How to compile:
 * gcc -o piprog -fopenmp -O3 piprog_A.c
 *
 * How to run:
 * OMP_NUM_THREADS=<num_threads> ./piprog
 *
 * 03.12.21, Alexander Blagodarnyi
 *
 * Results:
 * ----------------------------------------------------------------------------
 * |      times/#Threads      |       1       |       2       |       4       |
 * |--------------------------------------------------------------------------|
 * |  Elapsed parallel region |     92.60     |     46.32     |     32.20     |
 * |--------------------------------------------------------------------------|
 * |  Real time from time cmd |   1m32.605s   |   0m46.322s   |   0m32.200s   |
 * |--------------------------------------------------------------------------|
 * |    User time from cmd    |   1m32.488s   |   1m32.485s   |   1m32.457s   |
 * |--------------------------------------------------------------------------|
 * |       Interactive        |     87.19     |     45.22     |     33.86     |
 * |--------------------------------------------------------------------------|
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define NPOINTS		8000000000	// number trials
#define GENSEED		1235791		// prime random seed

int main(){

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
    unsigned long long i;

    srand48_r(rseed, &rbuf);

    #pragma omp for reduction(+:ncirc)  // split the work 
    for( i = 0; i < num_trials; i++ ){

	drand48_r(&rbuf, &dres1);      // re-entrant random num gen
	drand48_r(&rbuf, &dres2);      // [0..1)

	x = 2.0 * dres1 - 1;	       // place the circle around {0,0}
	y = 2.0 * dres2 - 1;
	t = x*x + y*y;

	if( t <= 1.0 ){
		ncirc++;
	}
    }
} // end parallel region

    double tend = omp_get_wtime();
    double tlaps = tend - tstart;

    pi = 4.0 * (double) ncirc/ (double) num_trials;
    dpi = pi*sqrt(2.0/num_trials);
    fprintf(stdout,"Trials: %ld Ncirc: %ld Threads: %d Elapsed: %.2f  PI: %.8lf dpi: %.1g\n",
		    num_trials, ncirc, size, tlaps, pi, dpi);

    return 0;
}
