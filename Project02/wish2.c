#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#define MAX_PARALLEL_COMMANDS 30
#define MAX_REDIRECTION_ACTIONS 5
#define TOTAL_ARGUMENTS_COMMAND 30

const char error_message[30] = "An error has occurred\n"; 

char *parallel_commands[MAX_PARALLEL_COMMANDS] = {NULL};
char *redirection[MAX_REDIRECTION_ACTIONS] = {NULL};
char *command_and_arguments[TOTAL_ARGUMENTS_COMMAND] = {NULL};

void get_parallel_commands(char *linePtr);
char *trim(char *str);

int main (int argc, char *argv[]){

    char *input_line = NULL;
    size_t len  = 0;
    ssize_t read;


    // Batch Mode
    if(argc == 2){

    }
    //Interactive Mode
    else if(argc == 1){
        
        while(1){
            printf("wish> ");
            read = getline(&input_line, &len, stdin);

            if(read == -1){
                write(STDERR_FILENO, error_message, strlen(error_message));
                exit(1);
            }

            input_line[strcspn(input_line, "\n")] = '\0';

            printf("A\n");
            get_parallel_commands(input_line);
            int j = 0;
            while(parallel_commands[j] != NULL){
                printf("B\n");
                printf("Comando %d: %s\n", j, parallel_commands[j++]);
            }
            printf("C\n");

        }
        
    }
    else{
        write(STDERR_FILENO, error_message, strlen(error_message));
        exit(1); 
    }



    free(input_line);
    return 0;
}

void get_parallel_commands(char *linePtr){

    for (int i = 0; i < MAX_PARALLEL_COMMANDS; i++) {
        parallel_commands[i] = NULL;
    }


    int count_commands = 0;
    char *copyLine = strdup(linePtr);
    char *cleanPtr = copyLine;

    if(!copyLine){
        write(STDERR_FILENO, error_message, strlen(error_message));
        exit(1);
    }

    char *token;
    while((token = strsep(&copyLine, "&")) != NULL){
        if (*token == '\0') continue;
        token = trim(token);
        parallel_commands[count_commands++] = strdup(token);
    }

    free(cleanPtr);
}

char *trim(char *str) {
    char *end;

    while (isspace((unsigned char)*str)) str++;

    if (*str == 0)
        return str;

    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    *(end + 1) = '\0';

    return str;
}