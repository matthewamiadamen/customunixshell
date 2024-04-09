/* Name: Matthew Amiadamen
*  Student No.: 22505553
*  Date: 23/03/24
*  File: utility.c
*  I have thoroughly read and understood the DCU Academic Integrity Policy
*  and I am aware of what the consequences of breaching them
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //posix library
#include <string.h>
#define MAX_BUFFER 1024
#define MAX_ARGS 64
#define SEPARATORS " \t\n"
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include "myshell.h"

void other(char **args){
    //command will store which system call string that i can now call to stdout
    char command[1024];
    strcpy(command, "which ");
    strcat(command, args[0]);
    char path_buffer[1024];

    //popen will not work if command is invalid
    FILE *fp = popen(command, "r");
    //read this into buffer
    if (fgets(path_buffer, sizeof(path_buffer), fp) == NULL){
        printf("Invalid Command\n");
        pclose(fp);
        return;
    }
    //now i close this pipe opened by popen()
    if(pclose(fp) == -1){
        perror("pclose");
        return;
    }
    //now trim \n form output
    path_buffer[strcspn(path_buffer, "\n")] = '\0';

    // Setting the environment variable parent=/myshell
    if (setenv("parent", "/myshell", 1) != 0) {
        perror("setenv error");
        return;
    }
    //check if there is an ampersand and it needs to be run in background
    int i = 0;
    while(args[i] != NULL){
        ++i;
    } 
    int flag = 0;
    //checks if last argument is ampersand or last argumetn ends in ampersand
    // strcpsn finds first occurnece of substring in string
    int len = strcspn(args[i - 1], "&");
    if(strcmp(args[i - 1], "&") == 0 || len == strlen(args[i - 1]) - 1){
        flag = 1;
        args[i - 1][len] = '\0';
    }

    //now create fork we have file path of executable aswell as arguments
    pid_t pid = fork();
    if(pid == 0){
        execvp(path_buffer, args);
        return;
    }
    else if(pid > 0){
        if(flag == 0){//!flag
        wait(NULL);
        }
        else{
            printf("Running in background\n");
        }
    }
}

//handles io redirection
void redirection(char **args) {
    pid_t pid = fork();
    if (pid == -1) { //make sure the fork works
        perror("fork");
        return;
    }

    if (pid == 0) { // Child process
        char *programme = args[0];
        int i = 0;
        char *params[64];
        int in = 0;
        int out = 0;

        // Constructing the params array
        while (args[i] != NULL && strcmp(args[i], "<") != 0 && strcmp(args[i], ">") != 0 && strcmp(args[i], ">>") != 0) {
            params[i] = args[i];
            ++i;
        }
        params[i] = NULL; // Parameters array null terminagted

        // this Handle input/output redirection
        while (args[i] != NULL) {
            if (!strcmp(args[i], "<")) {
                if ((in = open(args[i + 1], O_RDONLY)) < 0) {
                    fprintf(stderr, "Error opening file for input: %s\n", strerror(errno));
                    return;
                }
                //dup2 duplicates the input into the stdin file
                dup2(in, STDIN_FILENO);
                close(in);
            } else if (!strcmp(args[i], ">")) {
                if ((out = creat(args[i + 1], 0644)) < 0) {
                    fprintf(stderr, "Error creating file for output: %s\n", strerror(errno));
                    exit(EXIT_FAILURE);
                }
                dup2(out, STDOUT_FILENO);
                close(out);
            } else if (!strcmp(args[i], ">>")) {
                if ((out = open(args[i + 1], O_CREAT | O_RDWR | O_APPEND, 0644)) < 0) {
                    fprintf(stderr, "Error opening file for append: %s\n", strerror(errno));
                    exit(EXIT_FAILURE);
                }
                dup2(out, STDOUT_FILENO);
                close(out);
            }
            i += 2; // Incrementing by 2 to skip over the redirection symbol and filename
        }

        execvp(programme, params);
        fprintf(stderr, "Error in child execution: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    } else { // Parent process
        wait(NULL); // Parent waits for child to finish running before it continues
    }
}
