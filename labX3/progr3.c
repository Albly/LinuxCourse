
/* Program copies file in the first arg to second one or std streams 
 *
 * ------------------------------------------------------------------------------------
 * 
 * Author: Alex Blagodarnyi
 * date: 16.11.21
 * */
 
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

/* Program definitions */
#define SIZEBUF 128     // buffer size
#define OPTFLAGS "s:"   // option flags

extern int optind;
const char optflags[] = OPTFLAGS ;   //option flags


void usage(int argc, char *argv[]){
	fprintf(stderr,"usage: %s [-%c <int>] in out\n",argv[0],optflags[0]);
	exit(0);
}


long getparams(int argc, char *argv[]){
	const char *flags = optflags;  
	char *param = NULL;
	int opt;
	int flag = 0;
	extern char * optarg;
	long parsize = 0;
	
	// if no options: getopt returns -1
	while ((opt = getopt(argc, argv, flags)) != -1 ) { 
		
		if (opt == optflags[0] ) {    
				param = optarg; 			// current argv element
				parsize = atoll(param);		// convert str to long long integer    
		}
		// Shows usage and exit the program
		else usage(argc,argv);
	}
	return parsize;
}


int main(int argc, char *argv[]){

	int f1 = fileno(stdin); 		// file descriptor
	int f2 = fileno(stdout);
	int readflg = O_RDONLY; 
	int writflg = O_CREAT | O_WRONLY | O_TRUNC; 
	off_t fpos;
	ssize_t nread, nwrit;
	char buf[SIZEBUF];      					// predefined buffer size
	long parsize = getparams(argc, argv);		// get param size

	struct stat finfo;      					// structure with information about file

	fprintf(stderr,"option arguments: %ld\n",parsize);

	int k = optind;								// arg index
	if(k < argc) { 
		fprintf(stderr,"open stdin: %d %s\n",k, argv[k]);  
		f1 = open(argv[k],readflg); 			// open for reading
		if ( f1 < 0 ) perror("open input file");// prints message describing the error
	}

	k++;
	
	if(k < argc) { 
		fprintf(stderr,"open stdout: %d %s\n",k,argv[k]);  
		f2 = open(argv[k],writflg); 			  // open for writing
		if ( f2 < 0 ) perror("open output file"); // prints message describing the error

	}
	
	if ((fpos = lseek(f2, (off_t)0, SEEK_END))< 0) perror("lseek" );
	else fprintf(stderr,"write position in file %s is: %ld Bytes\n",argv[k], fpos);


	if ( (fpos = lseek(f2, parsize, SEEK_END)) < 0 ) perror("lseek" );
	else fprintf(stderr,"moving byound EOF by %ld Bytes, current position %ld Bytes\n",parsize, fpos);


	while ( (nread = read(f1, buf, SIZEBUF))  ) {    // get a str from stdin
		 nwrit = 0;
		 do {
			nread -= nwrit;
			nwrit = write(f2,buf + nwrit, nread);   // if write is interrupted it returns less bytes. 
		 } while ( nwrit < nread );					// then call is restarted until all bytes are written
        }

	if ( fstat(f2, & finfo) < 0 ) perror("fstat"); 	// fileno extracts file descriptior from FILE *
	else {
		fprintf(stderr,"file size: %ld\n",finfo.st_size);
		fprintf(stderr,"blocks allocated: %ld size on disk: %ld Bytes\n",
				 finfo.st_blocks, finfo.st_blocks * 512);
	}
	fprintf(stderr,"all done\n");
}
