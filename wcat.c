#include <stdio.h>

#define BUFFER_SIZE (4096)

int main(int argc, char *argv[]){

    if(argc == 1)   // Checks if the user typed any file name
        return 0;

        
    char buffer[BUFFER_SIZE];  // A buffer is declared to store file content

    for (int k = 1; k < argc; k++){                 // Loops for each file name typed by the user
        FILE *f = fopen(argv[k], "r");              // Opens the file

        if(!f){                                     // Checks if file opening was suscefull
            printf("wcat: cannot open file\n");
            return 1;
        }

        while(fgets(buffer, BUFFER_SIZE, f)){       // Copies the file content until it hits EOF
            printf("%s", buffer);                   // Print file content stored in the buffer to the standart output
        }

        fclose(f);                                  // Closes the file and desalocate resources
    }


    return 0;
}
