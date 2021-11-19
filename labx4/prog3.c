/*
 * program to demonstration allocation of memory 
 *
 * ------------------------------------
 * Alex Blagodarnyi.  Date: 18.11.21
 * Linux & Supercomputers course, Skoltech 2021
 */


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sysinfo.h>
#include <sys/mman.h>

#define MSIZE 1000000000
#define MiB    1.0/(1024*1024)
#define GiB    MiB/1024

typedef struct mys {
	int x;
	int y;
} mys_type;

unsigned long long freeram(){
	struct sysinfo info;
	unsigned long long ram = 0;
  
  	if (sysinfo(&info) < 0) perror("sysinfo");
	else ram = info.freeram;
	printf("free: %llu %.1f MiB mem unit: %d\n", info.freeram, (double)ram*MiB,  info.mem_unit);
	return ram;
}

long getparam(int argc, char *argv[]){
	long num_elements = MSIZE;
	const char flags[] = "s:";   
	char *param = NULL;

	int opt;
	extern int optind;
	extern char * optarg;

	while ( (opt = getopt(argc, argv, flags)) != -1 ) { 	
		switch (opt) {
		case 's': param = optarg; 
			  num_elements = atoll(param);
			  if ( num_elements > 0) break;
		default :
			 printf("invalid command flags: %s or parameter: %s\n",*argv, param);
			 exit(3);
		}
	}
	return num_elements;
}

int main(int argc, char *argv[]){

	const long pagesize = sysconf(_SC_PAGESIZE);          // get the default page size to use in allocation

	mys_type *data;
	long num_elements = getparam(argc, argv);
	size_t mys_bytes = num_elements * sizeof(mys_type);     // bytes needed to allocate that many elements	
	long pagesdef = 1 + (mys_bytes + pagesize -1)/pagesize; 

	unsigned long long freememory = freeram();
	unsigned long long freeleft, usedmem;
	int i;
    
	printf("Number of elements:  %d available memory: %.2f MiB\n",  num_elements, (double)freememory*MiB);
	if ( (mys_bytes <= 0) || (mys_bytes > freememory ) ) {
		printf("illegal numer of elements: %d, FreeMem: %u\n",num_elements,freememory);
		exit(4); 
	}

	mys_bytes = pagesdef * pagesize;               // allocate memory with integral #pages
	data = (mys_type *) mmap(NULL,mys_bytes, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANON, -1, (off_t)0); // read, write, map_shared, map_anonymous
	if ( data == NULL ) perror("mmap");
	else memset( data, (int)0xFF, mys_bytes); // set N bytes of S to C

	freeleft = freeram() ; // get free ram
	usedmem = freememory - freeleft; // used memory

	fprintf(stderr,"memory allocation size %lld Bytes %.1f MiB done Free: %llu %.1f MiB %llu %.1f MiB\n",mys_bytes,(double)mys_bytes*MiB,freeleft,(double)freeleft*MiB, usedmem, (double)usedmem*MiB); 
	
	sleep(10); // wait 10 seconds

	munmap(data,mys_bytes); //Deallocate any mapping for the region starting at ADDR and extending LEN bytes
}