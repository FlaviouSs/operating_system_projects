#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_ARGS 20

const char error_message[30] = "An error has occurred\n"; // Standart error menssage
const char *prompt_IM = "wish> "; // Prompt menssage for Interactive Mode
const char *prompt_BM = "(wish> )"; // Prompt menssage for Batch Mode
const char *exit_command = "exit"; // Exit Built-In command
const char *cd_command = "cd"; // Change Directory Built-In command

int change_directory_command(char *line);
void logic_get_command_and_arguments(char *line, char *command_and_args[]);
void logic_execute_command(char *command_and_args[]);
void clean_command_and_args(char *args[], int max);

int main(int argc, char *argv[]){

    char *line = NULL;
    char *command_args[MAX_ARGS];

    ssize_t nread;
    size_t size = 0;

    if(argc > 1){

    }
    else{
        while(1){
            write(1, prompt_IM, strlen(prompt_IM));
            nread = getline(&line, &size, stdin);

            if (nread == -1){
                write(1, error_message, sizeof(error_message));
                exit(1);
            }

            if (nread > 0 && line[nread - 1] == '\n') {
                line[nread - 1] = '\0';
            }

            if (strcmp(line, exit_command) == 0){
                exit(0);
            }
            
            if(change_directory_command(line)) continue;

            logic_get_command_and_arguments(line, command_args);
            logic_execute_command(command_args);

        }
        
    }

    return 0;
}

void logic_get_command_and_arguments(char *line, char *command_and_args[]){
    char *token;
    char *line_copy = strdup(line);

    if(line_copy == NULL){
        write(1, error_message, sizeof(error_message));
        exit(1);
    }

    int k = 0;
    while ((token = strsep(&line_copy, " ")) != NULL) {
        if (*token == '\0') continue;
        command_and_args[k++] = strdup(token);
    }
    command_and_args[k] = NULL;
    free(line_copy); 

}

void logic_execute_command(char *command_and_args[]){
    pid_t child_pid = fork();

    if(child_pid < 0){
        write(1, error_message, sizeof(error_message));
        exit(1);
    }
    else if(child_pid == 0){
        char *path = malloc(strlen("/bin/") + strlen(command_and_args[0]) + 1); 
        strcpy(path, "/bin/");
        strcat(path, command_and_args[0]);
        
        int check = access(path, X_OK);
        
        if(check == 0){
            execv(path, command_and_args);
        }else{
            free(path);
            
            path = malloc(strlen("/usr/bin/") + strlen(command_and_args[0]) + 1);
            strcpy(path, "/usr/bin/");
            strcat(path, command_and_args[0]);
            
            int check = access(path, X_OK);

            if(check == 0){
                execv(path, command_and_args);
            }else{ // Error
                write(1, error_message, sizeof(error_message));
                exit(1);
            }
        }
    }
    else{
        wait(NULL);
        clean_command_and_args(command_and_args, MAX_ARGS);
    }
}

void clean_command_and_args(char *args[], int max){
    for (int i = 0; i < max; i++) {
        if (args[i] != NULL) {
            args[i] = NULL;
        }
    }
}

int change_directory_command(char *line){
    char *token;
    char *line_copy = strdup(line);
    char *change_directory[3] = {NULL, NULL, NULL};

    if(line_copy == NULL){
        write(1, error_message, sizeof(error_message));
        exit(1);
    }


    int k = 0;
    while ((token = strsep(&line_copy, " ")) != NULL) {
        if (*token == '\0') continue;
        change_directory[k++] = strdup(token);
    }

    if(strcmp(change_directory[0], cd_command) == 0){
        if(change_directory[2] != NULL){
            write(1, error_message, sizeof(error_message));
            exit(1);
        }
        
        if(chdir(change_directory[1])){
            write(1, error_message, sizeof(error_message));
            exit(1);
        }

        for(int i = 0; i < 3; i++){
            if (change_directory[i] != NULL) {
                free(change_directory[i]);
            }
        }

        free(line_copy);
        return 1;
    }
    
    free(line_copy);
    return 0;
}