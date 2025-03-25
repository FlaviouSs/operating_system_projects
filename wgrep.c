#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE (4096)

int main(int argc, char *argv[]){

    FILE *f; 
    char buffer[BUFFER_SIZE];               // A buffer is declared to store data when the user doesn't type a file name
    char * bufferPtr = NULL;                // A buffer is declares to store data when the user types a file name
    long unsigned bufferPtr_SIZE = 0;       // It's impossible to know the size of line in a file. So we declare the buffer as NULL and its size is 0

    if(argc == 1){                          // Checks if the user typed any file name
        printf("wgrep: searchterm [file...]\n");
        return 1;
    }
    else if(argc == 2){                     // Recive data via standart input
        while(fgets(buffer, BUFFER_SIZE, stdin)){
            if(strstr(buffer, argv[1]))
                printf("%s", buffer);
        }
    }else{                                 // Recive data via files
        for(int k = 2; k < argc; k++){
            f = fopen(argv[k], "r");

            if(!f){
                printf("wgrep: cannot open file\n");
                return 1;
            }

            while(getline(&bufferPtr, &bufferPtr_SIZE, f) != -1){
                if(strstr(bufferPtr, argv[1]))
                    printf("%s", bufferPtr);
            }
        }
    }

    return 0;
}