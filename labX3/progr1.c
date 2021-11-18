/* Program copies file from the first argument to file in the second argument
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

/* Program definitions */
#define SIZEBUF 128     // Buffer size
#define OPTFLAGS "s:"   // possible flag

/* Global variables */
extern int optind;
const char optflags[] = OPTFLAGS;   //opt flags

void usage(int argc, char *argv[]){

	// prints usage for the program and exit
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
	while ((opt = getopt(argc, argv, flags)) != -1 ){ 
		// if opt is equal to OPTFLAGS	
		if (opt == optflags[0]) {    
				param = optarg; 	 		// current argv element
				parsize = atoll(param); 	// Convert string to long long integer  
		}
		// show usage and exit
		else usage(argc,argv);
	}
	return parsize;
}


int main(int argc, char *argv[])
{
	FILE *f1 = stdin;							// standard input stream
	FILE *f2 = stdout;							// standard output stream
	char buf[SIZEBUF];      					// predefined buffer size
	long parsize = getparams(argc, argv);		// param size
	int i;										// counter

	fprintf(stderr,"option arguments: %ld\n",parsize); 	// print option arguments 

	int k = optind;		// arg index
	
	if(k < argc) { 
		fprintf(stderr,"open stdin: %d %s\n",k, argv[k]);  
		f1 = freopen(argv[k],"r",stdin); 		// open for reading 
		printf("\n");    
		if ( f1 == NULL ) perror("open file on stdin");	// error, if file doesnt exits
	}

	k++;
	
	if(k < argc) { 
		fprintf(stderr,"open stdout: %d %s\n",k,argv[k]);  
		f2 = freopen(argv[k],"w",stdout); 				  // w - open for writing 
		printf("\n");   
		if ( f2 == NULL ) perror("open file on stdout");  // create if it does not exist 
	}

	while ( fgets(buf, SIZEBUF, f1)  ) {    	// get string from stdin
		buf[strlen(buf)-1] = '\0';      		// remove the trailing return-character '\n' 
		puts(buf); 
        }
	fprintf(stderr,"all done\n");    
}
