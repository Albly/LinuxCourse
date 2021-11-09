#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* This program prints "Hello, Skoltech". Takes -s as a flag with one argument
 * which is a name of some primitive type of C variable. Prints size of its type.
 * If no such type - prints -1
 * If more parameters were given: they will be printed.
 *
 * Possible types:
 * int , long, char, short, float, double 
 * ------------------------------------------------------------------------------------
 * 
 * Author: Alex Blagodarnyi
 * date: 05.11.21
 * */

extern void exit(int);

void usage(char *cmd){
        /*Prints how to use this script*/
        printf("%s argv -s type\n", cmd);
}

int main(int argc, char *argv[]){
        /*Function that will be run first */

        const char flags[] = "s:";      // possible flag with one parameter
        int opt;                        // current flag
        int size = 0;                   // var for size storing
        char *type = NULL;

        extern char * optarg;           // var, that inited in some other program
        extern int optind;
        extern char* optarg;

        while((opt = getopt(argc,argv,flags)) != -1){                           // read all input params
                if(opt == 's'){                                                 // if correct flag
                        type = optarg;                                          // write argument with name of type
                        printf("Argument: %s\n", type);
                }
                // Otherwise warns and exit
                else{
                        printf("Invalid command flags: %s\n", *argv);
                        exit(3);
                }
        }

        // If no argument -> print and exit
        if (type == NULL ){
                usage(argv[0]);
                exit(0);
        }

        // Find corresponding type and its size
        if (strcmp(type,"int") == 0) size = sizeof(int);
        else if (strcmp(type, "long") == 0 ) size = sizeof(long);
        else if (strcmp(type, "char") == 0) size = sizeof(char);
        else if (strcmp(type, "short") == 0) size = sizeof(short);
        else if (strcmp(type, "float") == 0) size = sizeof(float);
        else if (strcmp(type, "double") == 0) size = sizeof(double);
        else size = -1; // if no such type -> -1

        printf("Hello, Skoltech! Size of %s : %d\n", type, size);       // print hello, type and its size

        // prints other arguments
        if(optind < argc){
                printf("Other arguments: ");
        }

        // print these args one by one
        int i = optind;
        for(i; i<argc; i++){
                printf("%s ", argv[i]);
        }

        printf("\n");


}
