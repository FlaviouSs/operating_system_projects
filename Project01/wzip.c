#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main (int argc, char *argv[]){

    if(argc == 1){
        printf("wzip: file1 [file2 ...]\n");
        return 1;
    }

    //setbuf(stdout, NULL);

    int match = 0;
    char pivot_buffer = ' ';
    char next_buffer;

    for(int k = 1; k < argc; k++){

        FILE *f = fopen(argv[k], "r");

        if(match == 0){
            pivot_buffer = fgetc(f);
            if (pivot_buffer == EOF) {
                fclose(f);
                continue;
            }
            match = 1;
        }

        while((next_buffer = fgetc(f)) != EOF){
            if(pivot_buffer == next_buffer){
                match++;
            }else{
                fwrite(&match, sizeof(int), 1, stdout);
                fwrite(&pivot_buffer, sizeof(char), 1, stdout);
                //fflush(stdout);
                pivot_buffer = next_buffer;
                match = 1;
            }
        }

        fclose(f);
    }

    if (match > 0) {
        fwrite(&match, sizeof(int), 1, stdout);
        fwrite(&pivot_buffer, sizeof(char), 1, stdout);
        //fflush(stdout);
    }
    

    return 0;

}
