/* prog for demonstrationt the the RACE condition in access to shared variable
 * using atomic operation on the shared variable
 *
 * compile with gcc -o pthr pthr.c -pthread
 *
 * 
 * 02.12.21 Alexander Blagodarnyi
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdatomic.h>

#define NITER		500000000
#define NTHRS		16

atomic_long sum = 0;

void *func(void * arg){
    long n = *(long *) arg;
    long i;
    for (i=0; i<NITER; i++){
	sum += n;  
    }
    pthread_exit(NULL);
}

int main(int argc, char **argv){
    long na[NTHRS];
    pthread_t tid[NTHRS];
    pthread_attr_t attr;
    int nth = 1;
    char *cnth = getenv("NUM_THREADS");     //Return the value of envariable NAME, or NULL if it doesn't exist.
    int i;

    if(cnth) nth = atoi(cnth);              // Convert a string to an integer.
    if ( nth < 0 || nth >= NTHRS ) nth = 2;
    pthread_attr_init(&attr);               //initialize thread attribute attr with default attributes
    printf("starting %d threads: ",nth);
    for ( i=0; i < nth; i++){

	na[i] = (i & 1)? -1: 1;
        pthread_create(&(tid[i]), &attr, func, &(na[i]));  //creates thread
	printf("   [ %d: %x %d ]",i,tid[i],na[i]);
    }

    printf("\n");
    for ( i=0; i < nth; i++){
       pthread_join(tid[i], NULL);      //Make calling thread wait for termination of the thread tid[i]
    }
    printf("sum = %lld\n", sum);
    return 0;
}

