/* Program emulates behaviour of grep command: grep '[A-Z]' blackhol.txt | grep -v '[a-z]'
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
#define SIZEBUF 128     	// buffer size
#define OPTFLAGS "s:"   	// option flags

extern int optind;
const char optflags[] = OPTFLAGS ;   	// option flags

void usage(int argc, char *argv[]){
	fprintf(stderr,"usage: %s [-%c <int>] in out\n",argv[0],optflags[0]);
	exit(0);
}

long getparams(int argc, char *argv[]) {
	const char *flags = optflags;  
	char *param = NULL;
	int opt;
	int flag = 0;
	extern char * optarg;
	long parsize = 0;
	
	// if no options: getopt returns -1
	while ( (opt = getopt(argc, argv, flags)) != -1 ) { 		
		if ( opt == optflags[0] ) {    		// if flas is found 
				param = optarg; 			// put paramter from optarg to param
				parsize = atoll(param);		// convert str to long long integer    
		}

		// show usage and exit program
		else usage(argc,argv);
	}
	return parsize;
}


int main(int argc, char *argv[]){

	FILE *f1 = stdin;				// standard input stream
	FILE *f2 = stdout;				// standard output stream
	char buf[SIZEBUF];      		// predefined buffer size
	long parsize = getparams(argc, argv);	// get param size
	int llen = 0;					// length counter in input line
	int flagupper;	        		// lower case present
	int i;							// counter

	fprintf(stderr,"option arguments: %ld\n",parsize);

	int k = optind;	// arg index
	if(k < argc) { 
		fprintf(stderr,"open stdin: %d %s\n",k, argv[k]);  
		f1 = freopen(argv[k],"r",stdin); printf("\n");    	// open file for reading
		if ( f1 == NULL ) perror("open file on stdin");		// if file doesnt exist- error
	}

	k++;
	
	if(k < argc) { 
		fprintf(stderr,"open stdout: %d %s\n",k,argv[k]);  
		f2 = freopen(argv[k],"w",stdout); printf("\n");   	// open for writing
		if ( f2 == NULL ) perror("open file on stdout");	// if file doesnt exist- error 
	}

	while ( fgets(buf, SIZEBUF, f1)  ) {  				// get str from stdin  
		llen = strlen(buf);							
		buf[llen] = '\0';        						// remove the trailing return-character '\n' 
		for(flagupper = 0, i=0; i<llen; i++) {
			if ( isalnum(buf[i]) ) flagupper = 1;
			if ( islower(buf[i]) ) {
				flagupper = 0;
				break;
			}
		}
		if ( flagupper ) fputs(buf,f2);
        }
	fprintf(stderr,"all done\n");  
}
