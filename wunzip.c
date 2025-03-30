#include <stdio.h>

int main (int argc, char *argv[]){

    FILE *f = fopen(argv[1], "r");
    char readed_char;
    int match;

    if(argc == 1){
        printf("wunzip: file1 [file2 ...]\n");
        return 1;
    }
    
    fread(&match, sizeof(int), 1, f);
    fread(&readed_char, sizeof(char), 1, f);

    while(!feof(f)){
        for(int k = 1; k <= match; k++){
            printf("%c", readed_char);
        }

        fread(&match, sizeof(int), 1, f);
        fread(&readed_char, sizeof(char), 1, f);
    }

    fclose(f);
    return 0;
}