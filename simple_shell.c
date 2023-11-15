#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT_SIZE 100

void display_prompt() {
    printf("$ ");
}

void execute_command(char *command) {
    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid == 0) {
        // Child process
        if (execve(command, (char *const []){command, NULL}, NULL) == -1) {
    perror("simple_shell");
        }

        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        perror("simple_shell");
    } else {
        // Parent process
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
}

int main() {
    char input[MAX_INPUT_SIZE];

    while (1) {
        display_prompt();

        if (fgets(input, sizeof(input), stdin) == NULL) {
            // Handle Ctrl+D (end of file)
            printf("\n");
            break;
        }

        // Remove the trailing newline character
        input[strcspn(input, "\n")] = 0;

        if (strlen(input) == 0) {
            continue; // Empty command, display prompt again
        }

        execute_command(input);
    }

    return 0;
}

