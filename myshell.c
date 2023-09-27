#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>


/**
 * This defines a struct named CommandInfo that contains an integer pid and a character array command.
  */
typedef struct {
    int pid;
    char command[100];
} CommandInfo;

/**
 * The function takes a command string and splits it into tokens, storing them in an array and returning the number of
 * tokens.
 *
 * @param command A string containing the command to be executed, along with any arguments.
 * @param commandArgArray commandArgArray is a pointer to a pointer to a char (i.e. a pointer to a string array). It is
 * used to store the individual tokens (i.e. words) of the input command after it has been split by spaces.
 *
 * @return The function `commandReader` is returning the number of tokens (arguments) in the input command.
 */
int commandReader(char *command, char **commandArgArray) {
    // Split the input command into tokens
    int tokenCounter = 0;
    char *token = strtok(command, " ");
    // This code is splitting the input command string into individual tokens (words) using the `strtok()` function.
    while (token != NULL) {
        commandArgArray[tokenCounter] = token;
        tokenCounter++;
        token = strtok(NULL, " ");
    }
    // set the last argument to NULL
    commandArgArray[tokenCounter] = NULL;
    return tokenCounter;
}

/**
 * The function adds a command to a history array of CommandInfo structs.
 *
 * @param history A pointer to an array of CommandInfo structs, which represents the command history.
 * @param historyCount A pointer to an integer variable that stores the current count of commands in the history array.
 * @param command The `command` parameter is a pointer to a character array that represents the command to be added to the
 * history.
 */
void addCommandToHistory(CommandInfo *history, int *historyCount, char *command) {
    strncpy(history[*historyCount].command, command, sizeof(history[*historyCount].command) - 1);
    history[*historyCount].command[sizeof(history[*historyCount].command) - 1] = '\0';
    (*historyCount)++;
}

/**
 * The function prints the command history with the corresponding process ID.
 *
 * @param history A pointer to an array of CommandInfo structs, which contains the history of commands executed.
 * @param historyCount The number of elements in the `history` array.
 */
void getHistory(CommandInfo *history, int historyCount) {
    for (int i = 0; i < historyCount; i++) {
        printf("%d %s\n", history[i].pid, history[i].command);
    }
}

/**
 * The function changes the current working directory to the given path.
 *
 * @param path A pointer to a string that represents the path of the directory to which the current working directory needs
 * to be changed.
 *
 * @return an integer value, which is the result of the chdir() function. The chdir() function changes the current working
 * directory to the given path and returns 0 on success, or -1 on failure. Therefore, the cd() function is returning 0 on
 * success and -1 on failure.
 */
int cd(char *path) {
    // Change the current working directory to the given path
    int result = chdir(path);
    if (result != 0) {
        perror("chdir failed");
    }
    return result;
}

/**
 * The function adds a process ID to a command history array.
 *
 * @param history A pointer to an array of CommandInfo structs, which is used to store the history of executed commands.
 * @param historyCount A pointer to an integer variable that stores the current count of items in the history array.
 * @param pid The parameter `pid` is an integer representing the process ID (PID) of a command that has been executed.
 */
void pidAddToHistory(CommandInfo *history, int *historyCount, int pid) {
    int tempCounter = *historyCount - 1;
    history[tempCounter].pid = pid;
}


/**
 * The function takes user input for a shell command and ensures that it is not empty before proceeding.
 *
 * @param command The parameter "command" is a character array that will store the user input for the shell command.
 */
void shellCall(char command[]) {
    while (true) {
        printf("$ ");
        scanf("%[^\n]", command);
        getchar();
        // consume the newline character left in the input buffer
        fflush(stdout);
        if (command[0] != '\0') {
            break;
        }
    }
}


/**
 * This function adds the specified arguments to the PATH environment variable.
 *
 * @param argc The number of arguments passed to the function, including the name of the program itself.
 * @param argv The `argv` parameter is a pointer to an array of strings, where each string represents a command line
 * argument passed to the program. The first element of the array (`argv[0]`) is the name of the program itself.
 */
void addEnvironmentVar(int argc, char **argv) {
    char pathArray[1001] = {};
    char *path = getenv("PATH");
    if (path == NULL) {
        perror("getenv failed");
    }
    strcpy(pathArray, path);

    int i = 1;
    while (i < argc) {
        strcat(pathArray, " ");
        strcat(pathArray, argv[i]);
        i++;
    }
    int check = setenv("PATH", pathArray, 1);

    if (check == -1) {
        perror("setenv failed");
    }

}


int main(int argc, char **argv) {
    CommandInfo history[100];
    int historyCount = 0;
    // array to hold command arguments
    char *commandArgArray[100];
    addEnvironmentVar(argc, argv);
    while (true) {
        char command[101] = {};
        shellCall(command);
        // Save the command to history
        addCommandToHistory(history, &historyCount, command);
        commandReader(command, commandArgArray);
        if (commandArgArray[0] == NULL) {
            historyCount--;
            continue;
        }
        if (strcmp(command, "history") == 0) {
            pidAddToHistory(history, &historyCount, getpid());
            getHistory(history, historyCount);
            continue;
        }

        if (strcmp(command, "exit") == 0) {
            pidAddToHistory(history, &historyCount, getpid());
            exit(0);
        }

        if (strcmp(commandArgArray[0], "cd") == 0) {
            pidAddToHistory(history, &historyCount, getpid());
            cd(commandArgArray[1]);
            continue;
        }
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork failed");
            continue;
        }
        if (pid > 0) {
            pidAddToHistory(history, &historyCount, pid);
            // Parent process
            int result = wait(NULL);
            if (result == -1) {
                perror("wait failed");
            }
        } else if (pid == 0) {  // child process
            int check = execvp(commandArgArray[0], commandArgArray);
            if (check == -1) {
                perror("execvp failed");
                exit(1);
            }
        }
    }
    return 0;
}
