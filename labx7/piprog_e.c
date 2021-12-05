#include <stdio.h>
#include <stdlib.h>
#ifdef _OPENMP
#include <omp.h>
#else
#define omp_get_max_threads() 1
#define omp_get_num_threads() 1
#define omp_get_thread_num() 0
#define omp_get_wtime() 0
#endif

#define NPOINTS		1000000000	// number trials
#define GENSEED		1235791		// prime random seed

static long num_trials = NPOINTS;

long pihit(struct drand48_data *rbuf, long num_trials){			// note: num_trials is private!
    int size = omp_get_num_threads();
    int rank = omp_get_thread_num();
    double x, y, t, dres1, dres2;
    long ncirc = 0;
    long i;

    #pragma omp for
    for( i = 0; i < num_trials; i++ ){
    //for( i = rank; i < num_trials; i+= size ) {

	drand48_r(rbuf, &dres1);      // re-entrant random num gen
	drand48_r(rbuf, &dres2);      // [0..1)

	x = 2.0 * dres1 - 1;	       // place the circle around 0	
	y = 2.0 * dres2 - 1;
	t = x*x + y*y;

	if( t <= 1.0 ) ncirc++;
    }
    return ncirc;
}


int main(){

    long ncirc = 0;
    double pi;
    int size = omp_get_max_threads();

    double tstart = omp_get_wtime();

#pragma omp parallel default(none) firstprivate(num_trials) shared(ncirc)
{
    int rank = omp_get_thread_num();
    struct drand48_data rbuf;
    long rseed = (rank+1) * GENSEED;

    srand48_r(rseed, &rbuf);

    printf("firstprivate num_trials: %d adr: %lx\n", num_trials, &num_trials);
//    #pragma omp atomic
//    #pragma omp critical 
    ncirc  += pihit(&rbuf, num_trials);

}
    double tend = omp_get_wtime();
    double tlaps = tend - tstart;

    pi = 4.0 * (double) ncirc/ (double) num_trials;
    fprintf(stderr,"Trials: %ld Ncirc %ld on %d threads. T: %.2f  PI = %lf\n",
		    num_trials, ncirc, size, tlaps, pi);
    fprintf(stdout,"Trials %lld Ncirc %lld PI %lf\n",num_trials, ncirc, pi);
    return 0;
}
