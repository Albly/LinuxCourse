/*Program for demonstration of shell behaviour reading commands from stdin

* 23.11.21. Alexander Blagodarnyi
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define BUFSIZE     128  // size of input buffer 
#define CMDNAME      32  // maximum size of the command
#define NUMARGS      8  // maximum number of arguments
#define FULLCMD     255  // 

int main(int argc, char *argv[]){
    char buf[BUFSIZE];
    char cmd[BUFSIZE];
    char *cmadargs[NUMARGS] = {NULL};
    int params, totparams;
    int history = 0;
    int nparams = 0;
    int n;
    pid_t pid;
    int status; 
    int cpath; 
    char fullcmd[FULLCMD];
    char *PATH = getenv("PATH");
    char s1,s2;

    while(1){
        printf("%-d> ", history++);
        if (fgets(buf,BUFSIZE, stdin) == NULL){     //Get a newline-terminated string of finite length from STREAM
            break;
        }
        if (strlen(buf) == 0 ){
            continue;
        }
        for (nparams = 0; nparams<NUMARGS; nparams++){
            free(cmadargs[nparams]);                // Free a block allocated by `malloc', `realloc' or `calloc'.
            cmadargs[nparams] = 0;
        }
        totparams = params = nparams = 0;

        do{
            n = sscanf(buf+totparams, "%ms %n", &cmadargs[nparams], &params); // Read formatted input from S
            totparams += params;
            nparams ++;
        } while (n>0);

        cpath = 0;
        do{
            struct stat statbuf;
            int cmdlen = strlen(cmadargs[0]);  // Return the length of S.
            int cp2 = cpath;

            s1 = PATH[cpath];
            s2 = s1;
            while (s2 && (PATH[cpath] != ':'))
            {
                s2 = PATH[cpath++];
            }

            strncpy(fullcmd, &PATH[cp2], cpath-cp2);  //Copy no more than N characters of SRC to DEST
            fullcmd[cpath-cp2] = '/';
            cpath++;
            strcpy(&fullcmd[cpath-cp2], cmadargs[0]);

            if (stat(fullcmd, & statbuf) == 0){ // Get file attributes for FILE and put them in BUF
                break;
            }
        }while(s2);

        pid = fork();   // 
        if (pid<0){
            perror("fork");     // print a message describing the meaning of the value of errno
            continue;
        }
        if (pid == 0) {
            for (int i =0; i<nparams; i++){
                fprintf(stderr, "child cmd %s, %d parameter: %s\n", cmadargs[0], i, cmadargs[i]);
            }
            if (execve(fullcmd,cmadargs, NULL) < 0) { // eplace the current process, executing PATH with arguments ARGV
                perror("execve");
                exit(EXIT_FAILURE);
            }
        }
        waitpid(-1,&status,0); // Wait for a child matching PID to die.
        fprintf(stderr, "status execution of %s %d\n", cmadargs[0], status);
    }
    fprintf(stderr, "all done \n");
}