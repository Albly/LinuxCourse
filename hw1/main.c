#include <stdio.h>
#include <unistd.h>
#include <string.h>

extern void exit(int);
void usage(char *cmd){
    printf("%s -s type\n", cmd);
}

int main(int argc, char** argv){
    const char flags[] = "s:";
    int opt;
    int size = 0;
    char *type = NULL;

    extern char * optarg;
    extern int optind;
    extern char* optarg;

    while( (opt = getopt(argc, argv, flags)) != -1){
        switch (opt)
        {
        case 's':
            type = optarg;
            printf("argument: %s\n", type);
            break;
        
        default:
            printf("invalid command flags: %s\n", *argv);
            exit(3);
            break;
        }
    }

    if(type == NULL){usage(argv[0]); exit(0);}

    if(strcmp(type,"int") == 0) size = sizeof(int);
    else size = -1;

    printf("Hello, size of %s : %d\n", type,size);

    if(optind < argc) printf("other args:");
    for (int i=optind; i<argc; i++) printf("%s ", argv[i]);
    printf("\n");
}

