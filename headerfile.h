//
// Created by jamie on 5/6/23.
//

#ifndef SHELL_HEADERFILE_H
#define SHELL_HEADERFILE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
extern char **environ;

char *readLine(void);
char **splitLine(char *line);
int launch(char **args);
int execute(char **args);
void loop();

#endif //SHELL_HEADERFILE_H
