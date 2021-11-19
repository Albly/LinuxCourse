/*
 * program to demonstration allocation of memory with malloc
 * allocates all free memory and then frees it.
 *
 * ------------------------------------
 * Alex Blagodarnyi.  Date: 18.11.21
 * Linux & Supercomputers course, Skoltech 2021
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/sysinfo.h>
#include <errno.h>

#define MiB 1.0/(1024*1024)         // 2^20
#define GiB 1.0/(1024*1024*1024)    // 2^30

unsigned long long freeram(){
    /*Function gets amount of free ram and returns it*/
    struct sysinfo info;
    unsigned long long ram = 0;

    if (sysinfo(&info) < 0) perror("sysinfo"); // get system statistics
    else ram = info.freeram;                   // get amount of freeram
    return ram;

}

int main(){
    size_t mys_bytes = LONG_MAX;        // limits of long variable
    char * data;
    long ptr = NULL;
    long onegig = 1024*1024*1024;

    unsigned long long freememory = freeram(); 

    fprintf(stderr, "Available memory: %.2f GiB \n", (double) freememory*GiB);
    fprintf(stderr, "Asking allocation max size:  %ld %.2f GB\n", mys_bytes, mys_bytes*GiB);

    if ( malloc(mys_bytes) == NULL) perror ("malloc");  // allocate LONG_MAX mytes of memory
    fprintf(stderr, "successfuly allocated %ld %.2f Gb\n", mys_bytes, mys_bytes*GiB);

    do {
        memset(data+ptr, (int)0, (size_t)onegig);   // Set N bytes of S to C
        sleep(1);                                   // wait 1 sec
        ptr +=onegig;
        fprintf(stderr, "memory %10X-%10X initialized %10ld %.2f GB\n", data+ptr-onegig, data+ptr-1, ptr, ptr*GiB);
    }while(ptr < mys_bytes);
    free(data);
}