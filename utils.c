#include <malloc.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "headerfile.h"

//
// Created by jamie on 5/6/23.
//


#define READ_BUF_SIZE 1024
#define TOKEN_SIZE 64

int builtIns();


char *builtInStr[] = {"cd", "help", "exit"};

char *readLine(void) {
//    printf("readLine() -->");
    int bufSize = READ_BUF_SIZE;
    int pos = 0;
    char *buffer = malloc(sizeof(char) * bufSize);
    int ch;
    //check if allocation was successful
    if (!buffer) {
        fprintf(stderr, "shell: allocation was not successful");
        exit(EXIT_FAILURE);
    }
    while (1) {
        ch = getchar();
        if (ch == EOF || ch == "\n") {
            buffer[pos] = (char) "\0";
        } else {
            buffer[pos] = ch;
        }
        pos++;
        if (pos >= bufSize) {
            bufSize += READ_BUF_SIZE;
            buffer = realloc(buffer, bufSize);
            if (!buffer) {
                fprintf(stderr, "shell: reallocation failed");
                exit(EXIT_FAILURE);
            }
        }
//        printf("%s <--",buffer);
    }
}

#define TOKEN_DELIM " \n"

char **splitLine(char *line) {
    int bufSize = TOKEN_SIZE, position = 0;
    char **tokens = malloc(bufSize * sizeof(char *));
    char *token;
    char *copyLin = malloc(TOKEN_SIZE);
    strcpy((char *) *copyLin, (const char *) *line);
    if (!tokens) {
        fprintf(stderr, "shell: allocation error");
        exit(EXIT_FAILURE);
    }
    token = strtok(copyLin, TOKEN_DELIM);
    while (token != NULL) {
        tokens[position] = token;
        position++;
        if (position >= bufSize) {
            bufSize += TOKEN_SIZE;
            tokens = realloc(tokens, bufSize * sizeof(char *));
            if (!tokens) {
                fprintf(stderr, "shell: reallocation error");
                exit(EXIT_FAILURE);
            }
        }
        token = strtok(NULL, TOKEN_DELIM);
    }
    tokens[position] = NULL;
    return tokens;
}


int launch(char **args) {
    pid_t pid, wpid;
    int status;
    pid = fork();
    if (pid == 0) {

        if (execve(args[0], args, environ) == -1) { //child process
            perror("shell");
        } else {
            do {
                wpid = waitpid(pid, &status, WUNTRACED);
            } while (!WIFEXITED(status) && !WIFSIGNALED(status));
        }
    }
    return 1;
}


int cdDir(char **args) {
    //check if arg is null
    if (args[1] == NULL) {
        fprintf(stderr, "shell: Expected argument cd");
    } else {
        if (chdir(args[1]) != 0) {
            perror("shell");
        }
    }
    return 1;
}

int help(char **args) {
    int k;
    printf("My shell \n");
    printf("You need to type in commands and args and hit enter");
    printf("Here are the built in funcs");
    for (int i = 0; i < builtIns(); i++) {
        printf(" %s\n", builtInStr[i]);
    }
    printf("You can get more commands using man command");
    return 1;
}

int mExit(char **args) {
    return 0;
}


int (*builtInFunc[])(char **) = {&cdDir, &help, &mExit};

int builtIns() {
    return sizeof(builtInStr) / sizeof(char *);
}

int execute(char **args) {
    printf("execute() -->");
    if (args[0] == NULL) {
        return 1;
    }
    for (int k = 0; k < 3; k++) {
        if (strcmp(args[0], builtInStr[k]) == 0) {
            return (*builtInFunc)(args);
        }
    }
    return launch(args);
}

void loop(){
    char *line;
    char **toks;
    int status;
    do {
        printf("$shell >");
        line = readLine();
        toks = splitLine(line);
        status = execute(toks);

        free(line);
        free(toks);
    } while (status);
}

