/*
 * program to demonstration allocation of memory with malloc
 * invocation:   prog -s <long>
 * the parameter indicates the size of allocation in elements of structure mys_type
 *
 * ------------------------------------
 * Alex Blagodarnyi.  Date: 18.11.21
 * Linux & Supercomputers course, Skoltech 2021
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sysinfo.h>                               // definitions for sysinfo

#define MSIZE  1000000
#define MiB    1.0/(1024*1024)
#define GiB    1.0/(1034*1024*1024)

#define SIZEBUF 128         // input buffer size
#define OPTFLAGS "s:"       // opt flag with argument


typedef struct mys {    
    //  Structure for demonstration the concept
	double x;
	double y;
} mys_type; // init it 

const char optflags[] = OPTFLAGS ;   //opt flags

void usage(int argc, char *argv[]){
        fprintf(stderr,"usage: %s [-%c <int>] \n",argv[0],optflags[0]);
        exit(0);
}

long getparams(int argc, char *argv[]){
        const char *flags = optflags;
        char *param = NULL;
        int opt;
        int flag = 0;
        extern char * optarg;
        long parsize = MSIZE;

        while ( (opt = getopt(argc, argv, flags)) != -1 ) {
                if ( opt == optflags[0] ) {     
                                 param = optarg;
                                 parsize = atoll(param);
		}
	}
	return parsize;
}

unsigned long long freeram(){                      // gets available memory from sys
	struct sysinfo info;
	unsigned long long ram = 0;
  
  	if (sysinfo(&info) < 0) perror("sysinfo");     // get the system info
	else ram = info.freeram ;
	printf("free: %llu %.1f GB mem unit: %d\n", info.freeram, (double)ram*GiB,  info.mem_unit);
	return ram;
}


int main(int argc, char *argv[]){
	long num_elements = getparams(argc,argv); // get parameter

	unsigned long long freememory = freeram(); // get free mem
	unsigned long long freeleft, usedmem;

	size_t mys_bytes;
	mys_type *data;

	printf("Number of elements:  %d available memory: %.2f MB\n",  num_elements, (double)freememory*MiB);

	mys_bytes = num_elements * sizeof(mys_type);
	if ( (mys_bytes <= 0) || (mys_bytes > freememory ) ) {
		fprintf(stderr,"illegal numer of elements: %d, FreeMem: %u\n",num_elements,freememory);
		exit(4); 
	}

	data = (mys_type *) malloc(mys_bytes); // allocate memory
	if ( data == NULL ) perror("malloc");
	

	int i=0;
	for( i=0; i < num_elements; i++ ) {
		data[i].x = 1.0;    // add elements to struct
		data[i].y = 2.0;
	}

	freeleft = freeram() ; // get free memory
	usedmem = freememory - freeleft; // calculate used mem

	fprintf(stderr,"memory allocation size %lld Bytes %.1f MiB done. Used memory: %llu Bytes %.1f MiB\n",
		mys_bytes,(double)mys_bytes*MiB, usedmem, (double)usedmem*MiB); 
	
	sleep(10); // wait 10 sec

	free(data); // free a block allocated by malloc
}
