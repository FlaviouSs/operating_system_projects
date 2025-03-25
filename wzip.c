#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main (int argc, char *argv[]){

    if(argc == 1){
        printf("wzip: file1 [file2 ...]\n");
        return 1;
    }

    for(int k = 1; k < argc; k++){

        FILE *f = fopen(argv[k], "r");

        int match = 0;
        char pivot_buffer = ' ';
        char next_buffer;
        char *write_buffer = malloc(20);

        pivot_buffer = fgetc(f);
        match++;
        if(pivot_buffer == EOF){
            sprintf(write_buffer, "%d%c", match, pivot_buffer);
            fwrite(write_buffer, strlen(write_buffer), 1, stdout);
            fclose(f);
        }
        while((next_buffer = fgetc(f)) != EOF){
            if(pivot_buffer ==next_buffer){
                match++;
            }else{
                sprintf(write_buffer, "%d%c", match, pivot_buffer);
                fwrite(write_buffer,strlen(write_buffer), 1, stdout);
                pivot_buffer = next_buffer;
                match = 1;
            }
        }

    }


    return 0;

}
