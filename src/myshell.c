/* Name: Matthew Amiadamen
*  Student No.: 22505553
*  Date: 23/03/24
*  File: myshell.c
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
#include "myshell.h"

int main(int argc, char *argv[]){
    //check for batchfile
    //stat is a library struct which checks if said string is a file
    struct stat q;
    FILE* input = NULL;
    if(argc == 2 && stat(argv[1], &q) == 0){
        //if it meets conditons input becomes the file rather than stdin
        //everything else runs as usual
        input = fopen(argv[1], "r");
        if(input == NULL){
            perror("Error opening file");
            return 1;
        }
    }
    else{
        input = stdin;
    }

    int n = 8;
    //memory allocation for names of functions and function pointers
    void (**functionlist)(char**) = malloc(n * sizeof(void (*)(char**)));
    char **functionnames = malloc(n * sizeof(char *));
    for(int i = 0; i < n; ++i){
        functionnames[i] = malloc(100 * sizeof(char));
    }
    functionlist[0] = clr;
    functionlist[1] = dir;
    functionlist[2] = print_environ;
    functionlist[3] = quit;
    functionlist[4] = cd;
    functionlist[5] = echo;
    functionlist[6] = pauce;
    functionlist[7] = help;

    //each function name correspongs to a function pointer
    //this means we can scan for a function name and dynamically run it
    // and also means especially with shell if there are clashing functinon names
    // from standard libraries we can change the function pointer name but not the function name the user will type
    strcpy(functionnames[0], "clr");
    strcpy(functionnames[1], "dir");
    strcpy(functionnames[2], "environ");
    strcpy(functionnames[3], "quit");
    strcpy(functionnames[4], "cd");
    strcpy(functionnames[5], "echo");
    strcpy(functionnames[6], "pause");
    strcpy(functionnames[7], "help");

    char buf[MAX_BUFFER];
    char * args[MAX_ARGS];
    char ** arg;
    char prompt[1024];

    //beginning of user input 
    while(!feof(input)){
        //we get the enviroment variale for dir path to use as prompt
        strcpy(prompt, getenv("PWD"));
        strcat(prompt, "$ ");
        fputs(prompt, stdout);
        
        if(fgets (buf, MAX_BUFFER, input )){//this reads the line
            //now we must change line into arguments array
            arg = args;
            *arg++ = strtok(buf, SEPARATORS);
            while((*arg++ = strtok(NULL, SEPARATORS)));

        }
        //stat is a library struct which checks if said string is a file
        struct stat s;
        int done = 0;
        for(int i = 0; args[i] != NULL; ++i){
            //if args[0] is a file and one of the arugments is related to io redirection
            if(stat(args[0], &s) == 0 && (strcmp(args[i], "<") == 0 || strcmp(args[i], ">") == 0 || strcmp(args[i], "<<") == 0)){
                redirection(args);
                done = 1;
                break;
            }
        }
        if(args[0] && done == 0){

            int try = 0;
            //checks stdin through list to see if its a command
            for(int i = 0; i < n; ++i){
                if(strcmp(args[0], functionnames[i]) == 0){
                    functionlist[i](args);
                    try = 1;
                    break;
                }
            }
            //checks try flag and so if no other function has ran it activates execvp and tries running there
            if(try == 0){
                other(args);
            }

            arg = args;
            //fputs("\n", stdout);
        }

    }
    
    //free all memory
    for(int i = 0; i < n; ++i){
        free(functionnames[i]);
    }
    free(functionlist);
    free(functionnames);
    return 0;
}
