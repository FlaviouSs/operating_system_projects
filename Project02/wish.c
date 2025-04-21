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
char *line = NULL; // Buffer to command line input
char *command_args[MAX_ARGS];

void clean_args(char *args[], int max);

int main(int argc, char *argv[]){

    ssize_t nread; // Variable used to see te return value of getline()
    size_t size = 0; // Variavle used to 

    if(argc > 1){ // Checks arguments to Batch Mode

    }else{ // Starts ./wish in Interactive Mode
        while(1){
            write(1, prompt_IM, strlen(prompt_IM)); // Writes 'wish> '
            nread = getline(&line, &size, stdin); // Read command line

            if (nread == -1){ // Checks if getline() fails
                write(1, error_message, sizeof(error_message));
                exit(1);
            }

            if (nread > 0 && line[nread - 1] == '\n') { // Checks if line ends with a break line, if yes, changes to \0
                line[nread - 1] = '\0';
            }

            if (strcmp(line, exit_command) == 0){ // Checks if user typed exit command
                break;
            }
            
            char *token; // Variable to store the pieces of our line
            char *command; // Varible to store the command of our line
            char *line_copy = strdup(line); // Copy of our line

            int k = 0;
            while ((token = strsep(&line_copy, " ")) != NULL) { // We're breaking our line based on withespaces
                if (*token == '\0') continue; // Ignores double whitespaces
                if (k == 0) { // Getting the command
                    command = token;
                }
                command_args[k++] = token; // Getting the arguments of the command
            }
            command_args[k] = NULL; // Command arry fineshes with NULL
            free(line_copy); 

            pid_t pid_filho = fork(); // Creates another process

            if(pid_filho < 0){
                write(1, error_message, sizeof(error_message));
                exit(1);
            }
            else if(pid_filho == 0){
                char *path = malloc(strlen("/bin/") + strlen(command) + 1); // Allocates memory to store the path of the command
                strcpy(path, "/bin/");
                strcat(path, command); // Stores command path to 'path'
                int check = access(path, X_OK); // Checks process access permission to the path
                
                if(check == 0){ // If permission is given, executes
                    execv(path, command_args);
                }else{ // If permission is not given, tries the other path
                    free(path);
                    path = malloc(strlen("/usr/bin/") + strlen(command) + 1);
                    strcpy(path, "/usr/bin/");
                    strcat(path, command);
                    int check = access(path, X_OK);

                    if(check == 0){ // If permission is given, executes
                        execv(path, command_args);
                    }else{ // Error
                        write(1, error_message, sizeof(error_message));
                        exit(1);
                    }
                }
                
            }
            else{
                wait(NULL); // Wait for children process to finish
                clean_args(command_args, MAX_ARGS); // Cleans command arguments array
            }

        }
        
    }

    return 0;
}

void clean_args(char *args[], int max){
    for (int i = 0; i < max; i++) {
        if (args[i] != NULL) {
            args[i] = NULL;
        }
    }
}
