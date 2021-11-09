#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* This program prints "Hello, Skoltech" and a value corresponding to the input flag.
 * In case of several flags, returns the value of the last one. 
 *
 * Possible flags:
 * -a : prints value 1
 * -b : prints value 2
 * -c : prints value 3
 * For other flags : warns, that it's not exist 
 * ------------------------------------------------------------------------------------
 * 
 * Author: Alex Blagodarnyi
 * date: 05.11.21
 * */


int main(int argc, char *argv[]){

        /*First function that will be run*/

        const char flags[] = "abc";                                     // all possible flags
        int opt;                                                        // current flag
        int flag = 0;                                                   // value to print

        while((opt = getopt(argc,argv,flags)) != -1){                   // read all input flags
                // Find corresponding value to the flag:
                if(opt == 'a')
                        flag = 1;

                else if(opt == 'b')
                        flag = 2;

                else if(opt == 'c')
                        flag = 3;

                // If such flag doesn't exist --> print warning and exit
                else{
                        printf("Invalid command flags: %s\n", *argv);
                        exit(3);
                }
        }

        printf("Hello, Skoltech! Flag = %d\n", flag);                   // prints hello and value of the given flag
}

