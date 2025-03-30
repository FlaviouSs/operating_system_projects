#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc == 1) {
        printf("wunzip: file1 [file2 ...]\n");
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        FILE *f = fopen(argv[i], "r");

        char readed_char;
        int match;

        fread(&match, sizeof(int), 1, f);
        fread(&readed_char, sizeof(char), 1, f);

        while (!feof(f)) {
            for (int j = 1; j <= match; j++) {
                printf("%c", readed_char);
            }

            fread(&match, sizeof(int), 1, f);
            fread(&readed_char, sizeof(char), 1, f);
        }

        fclose(f);
    }

    return 0;
}
