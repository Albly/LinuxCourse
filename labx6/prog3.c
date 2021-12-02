/*program for demonstrating RACE condition in access to shared variable
 * compile with gcc -o pthr pthr.c -pthreads
 *
 * 02.12.21 Alexander Blagodarnyi
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NITER		500000000

static long long sum = 0;

void *func(void * arg){
    long n = *(long *) arg;
    long i;
    for (i=0; i < NITER; i++) {
	sum = sum + n;
    }
    pthread_exit(NULL);     // Terminate calling thread.
}

int main(int argc, char **argv){
    long n1, n2 ;
    pthread_t tid1, tid2;
    pthread_attr_t attr;

    pthread_attr_init(&attr);   //Initialize thread attribute attr with default attributes
	n1 =  1;
    pthread_create(&tid1, &attr, func, &n1); // create thread
	n2 = -1;
    pthread_create(&tid2, &attr, func, &n2); // create thread
    pthread_join(tid1, NULL);   //Make calling thread wait for termination of the thread tid1
    pthread_join(tid2, NULL);   // Make calling thread wait for termination of the thread tid2

    printf("sum = %lld\n", sum);
    return 0;
}

