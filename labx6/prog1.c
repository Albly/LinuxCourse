/* Demonstrates work of two processes: parent and child

* 02.12.21 Alexander Blagodarnyi
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

main(){
	int pfd[2]; 
	if ( pipe(pfd) < 0) { 	// Create a one-way communication channel (pipe)
		perror(" pipe "); 
		exit(1); 
	}

	int pid = fork();	// Create a child process. pid is 1 if parrent, 0 if child

	if (pid < 0) { 
		perror(" fork " ); 
		exit(2); 
	}

	if(pid){
		close(pfd[0]);		// write side
	}
	   
	else{
		close(pfd[1]);  	// read side
	}

	while(1){
		int inchar;
		int n; 
		if(pid==0){
			/* Parent code*/
			n = read(pfd[0], &inchar, sizeof(inchar)); 	// Read NBYTES into BUF from FD. Return the number read
			printf("%6d got %d n=%d\n", pid, inchar,n);
		}
		else{   				
			/* Child code*/
			//inchar = getchar();
			scanf("%d",&inchar);	// Read formatted input from stdin.
			printf("%6d  send %d n=%d\n", pid, inchar,n);
			sleep(1); //wait 1 second
			n = write(pfd[1],&inchar,sizeof(inchar)); //Write N bytes of BUF to FD. Return the number written
		}
	}
	return 0;
}
