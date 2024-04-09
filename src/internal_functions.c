/* Name: Matthew Amiadamen
*  Student No.: 22505553
*  Date: 23/03/24
*  File: internal_functions.c
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

//change current directory
void  cd(char **args){
    if(args[1] == NULL){
        fprintf(stderr, "cd: missing arguments\n");
        return;
    }
    char command[264] = "";
    int i = 1;
    //string manipulation to get correct input
    while(args[i] != NULL){
        strcat(command, args[i]);
        if(args[i + 1] != NULL){
            strcat(command, " ");
        }
        i += 1;
    }  
    //check if the command is valid then reset enviroment variable
    
    if(access(command, F_OK) == 0){
        chdir(command);
        setenv("PWD", command, 1);
    }
    else{
        fprintf(stderr, "Directory %s does not exist\n", command);
    }
}

//clear screen
void clr(char **args){
    system("clear");
}

//show directory contentes
void dir(char **args){
    system("ls -al");

}

//show curr enviroment variables
void print_environ(char **args){
    extern char **environ;
    int i;
    for (i = 0; environ[i] != NULL; i++)
        printf("%s\n",environ[i]);
}

//echo display followed by newline
void echo(char **args){
    for(int i = 1; args[i] != NULL; ++i){
        printf("%s ", args[i]);
    }
    printf("\n");
}

//display user manual
void help(char **args){
    system("more ../manual/readme.md");
}

//this pauses shell until enter is pressed, called pauce because 
// of function name conflicts
void pauce(char **args){
    //used termios libary to create new terminal settings
    struct termios old_term, new_term;
    tcgetattr(STDIN_FILENO, &old_term);
    new_term = old_term;

    //changed terminal settings of new term turned off echoing
    //and canonical
    new_term.c_lflag &= ~(ICANON | ECHO);

    tcsetattr(STDIN_FILENO, TCSANOW, &new_term);
    char c;
    //now while stdin is without error and c is not enter it does notgint
    while(read(STDIN_FILENO, &c, 1) == 1 && c != '\n'){

    }
    //settings reverted back to normal
    tcsetattr(STDIN_FILENO, TCSANOW, &old_term);
    printf("\n");
}

void quit(char **args){
    exit(0);
}
