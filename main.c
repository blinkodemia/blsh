#include <errno.h>
#include <stdio.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void parseCommand(char *cmd, char **parameters);
int executeCommand(char **parameters);

#define MAXCHARS 512
#define MAXPARAMS 255

int main(void) {
    char command[MAXCHARS];
    char *parameters[MAXPARAMS];
    char prompt[MAXCHARS];

    rl_bind_key(
        '\t',
        rl_complete); // Set readline to autocomplete when pressing <TAB>
    system("clear");  // Clear screen after starting the program.

    while (1) {
        char *username = getenv("USER");
        char *currentDir = getenv("PWD");

        snprintf(prompt, sizeof(prompt), "%s @ %s > ", username, currentDir);
        readline(prompt);

        /* if (fgets(command, sizeof(command), stdin) == NULL)
              break; */
        if (command[strlen(command) - 1] == '\n') {
            command[strlen(command) - 1] = '\0';
        }

        parseCommand(command, parameters); // Parse commands and parameters

        if (strcmp(parameters[0], "exit") == 0)
            break; // Compare if the input is equal to "exit", then quit the
                   // program.
        if (executeCommand(parameters) == 0)
            break; // Execute command
    }

    return 0;
}

void parseCommand(char *command, char **parameters) {
    for (int i = 0; i < MAXPARAMS; i++) {
        parameters[i] = strsep(&command, " ");
        if (parameters[i] == NULL)
            break;
    }
}

int executeCommand(char **parameters) {
    pid_t pid = fork(); // Fork processes

    // Error
    if (pid == -1) {
        char *error = strerror(errno);
        printf("fork: %s\n", error);

        return 1;
    }
    // Child process
    else if (pid == 0) {
        // Execute command
        execvp(parameters[0], parameters);

        // Error occurred
        char *error = strerror(errno);
        printf("shell: %s: %s\n", parameters[0], error);
        return 0;
    }

    // Parent process
    else {
        // Wait for child process to finish
        int childStatus;
        waitpid(pid, &childStatus, 0);
        return 1;
    }
}
