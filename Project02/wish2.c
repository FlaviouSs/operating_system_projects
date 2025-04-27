#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/wait.h>

#define MAX_PARALLEL_COMMANDS 30
#define MAX_REDIRECTION_ACTIONS 10
#define TOTAL_ARGUMENTS_COMMAND 30

const char error_message[30] = "An error has occurred\n";

char *parallel_commands[MAX_PARALLEL_COMMANDS] = {NULL};
char *redirection[MAX_REDIRECTION_ACTIONS] = {NULL};
char *command_and_arguments[TOTAL_ARGUMENTS_COMMAND] = {NULL};


char ** get_arguments_only(char *command_and_arguments[]);
char * concat_strings(const char* str1, const char* str2);
char * checkPath(const char *current_path, const char *command);
void execute_command(char *command_and_arguments[], const char *redirection_file, const char *current_path);
void get_command_arguments(char *command_and_arguments_line);
void get_redirection_on_command(char *command_line);
void get_parallel_commands(char *linePtr);
char *trim(char *str);

int main (int argc, char *argv[]){

    char *path = "/bin";
    char *input_line = NULL;
    size_t len  = 0;
    ssize_t read;


    // Batch Mode
    if(argc == 2){

    }
    //Interactive Mode
    else if(argc == 1){
        
        while(1){
            write(1, "wish> ", strlen("wish> "));
            read = getline(&input_line, &len, stdin);

            if(read == -1){
                write(STDERR_FILENO, error_message, strlen(error_message));
                exit(1);
            }

            input_line[strcspn(input_line, "\n")] = '\0';

            get_parallel_commands(input_line); // Fills 'parallel_commands' array with pointers to each command in parallel
            
            pid_t children_pids[MAX_PARALLEL_COMMANDS];
            int num_commands = 0; 

            int j = 0;
            while(parallel_commands[j] != NULL){; // Moving upon each command pointer at 'parallel_commands' array
                get_redirection_on_command(parallel_commands[j]); // Fills redirection array with command and redirection file of the command at position j in 'parallel_commands'
                // Now, redirection array must contain at position 0 the command and it's arguments
                // If there is a redirection file, it must be at position 1 at the redirection array. Otherwise, 1 is NULL
                get_command_arguments(redirection[0]); // Fills command and arguments array. Starting from position 1, each position is an argument of the command.
                
                
                pid_t pid = fork();
                
                if(pid == 0){
                    execute_command(command_and_arguments, redirection[1], path);
                }
                else if(pid > 0){
                    children_pids[num_commands];
                    num_commands++;
                    wait(NULL);
                }
                else{
                    write(STDERR_FILENO, error_message, strlen(error_message));
                    exit(1);
                }

                j++;

            }

        }
        
    }
    else{
        write(STDERR_FILENO, error_message, strlen(error_message));
        exit(1); 
    }



    free(input_line);
    return 0;
}

/*
char ** get_arguments_only(char *command_and_arguments[]){
    int count = 0;

    while (command_and_arguments[count] != NULL) {
        count++;
    }

    if (count <= 1) {
        return NULL;
    }

    char **args_only = malloc(count * sizeof(char*)); // count-1 args + 1 NULL
    if (args_only == NULL) {
        write(STDERR_FILENO, error_message, strlen(error_message));
        exit(1);
    }

    for (int i = 1; i < count; i++) {
        args_only[i-1] = command_and_arguments[i];
        if (args_only[i-1] == NULL) {
            write(STDERR_FILENO, error_message, strlen(error_message));
            exit(1);
        }
    }

    args_only[count-1] = NULL;

    return args_only;

}

*/

char * concat_strings(const char* str1, const char* str2) {

    int total_length = strlen(str1) + strlen(str2) + 2;

    char* result = (char*)malloc(total_length * sizeof(char));
    
    if (result == NULL) {
        write(STDERR_FILENO, error_message, strlen(error_message));
        exit(1);
    }

    strcpy(result, str1);
    strcat(result, "/");
    strcat(result, str2);

    return result;
}

char * checkPath(const char *current_path, const char *command){
    int check = -1;
    char *copyLine = strdup(current_path);
    char *cleanPtr = copyLine;

    char *token;
    while((token = strsep(&copyLine, ":"))){

        char *auxiliar_path = concat_strings(token, command);

        check = access(auxiliar_path, X_OK);

        if(check == 0){
            free(cleanPtr);
            return auxiliar_path;
        }
    }

    if(check == -1){
        free(cleanPtr);
        write(STDERR_FILENO, error_message, strlen(error_message));
        exit(1);
    }
}

void execute_command(char *command_and_arguments[], const char *redirection_file, const char *current_path){

    if(redirection_file != NULL){
        int fd = open(redirection_file, O_WRONLY | O_CREAT | O_TRUNC, 0666);

        if(fd < 0){
            write(STDERR_FILENO, error_message, strlen(error_message));
            exit(1);
        }

        dup2(fd, STDOUT_FILENO);
        dup2(fd, STDERR_FILENO);

        close(fd);
    }

    char *path = checkPath(current_path, command_and_arguments[0]);

    printf("PATH: %s\n", path);
    printf("Comando: %s\n", command_and_arguments[0]);
    printf("Argumentos: %s\n", command_and_arguments[1]);
    execv(path, command_and_arguments);
}

void get_command_arguments(char *command_and_arguments_line){
    
    for (int i = 0; i < TOTAL_ARGUMENTS_COMMAND; i++) {
        command_and_arguments[i] = NULL;
    }

    int count_arguments = 0;
    char *copyLine = strdup(command_and_arguments_line);
    char *cleanPtr = copyLine;

    if(!copyLine){
        write(STDERR_FILENO, error_message, strlen(error_message));
        exit(1);
    }

    char *token;
    while((token = strsep(&copyLine, " ")) != NULL){
        if(*token == '\0') continue;
        token = trim(token);
        command_and_arguments[count_arguments++] = strdup(token);
    }

    free(cleanPtr);
}

void get_redirection_on_command(char *command_line){
    
    for (int i = 0; i < MAX_REDIRECTION_ACTIONS; i++) {
        redirection[i] = NULL;
    }

    int count_redirections = 0;
    char *copyLine = strdup(command_line);
    char *cleanPtr = copyLine;

    if(!copyLine){
        write(STDERR_FILENO, error_message, strlen(error_message));
        exit(1);
    }

    char *token;
    while((token = strsep(&copyLine, ">")) != NULL){
        token = trim(token);
        redirection[count_redirections++] = strdup(token);
    }

    for(int k = 2; k < MAX_REDIRECTION_ACTIONS; k++){
        if(redirection[k] != NULL){
            write(STDERR_FILENO, error_message, strlen(error_message));
            exit(1);
        }
    }

    free(cleanPtr);
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