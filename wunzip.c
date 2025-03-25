#include <stdio.h>

int main (int argc, char *argv[]){

    FILE *f = fopen(argv[1], "r");
    char readed_char;
    int match;

    while(fscanf(f, "%d%c", &match, &readed_char) != EOF){
        for(int k = 0; k < match; k++){
            printf("%c", readed_char);
        }
    }

    printf("\n");
    fclose(f);

    return 0;
}