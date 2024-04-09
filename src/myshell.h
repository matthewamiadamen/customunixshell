/* Name: Matthew Amiadamen
*  Student No.: 22505553
*  Date: 23/03/24
*  File: myshell.h
*  I have thoroughly read and understood the DCU Academic Integrity Policy
*  and I am aware of what the consequences of breaching them
*/
#ifndef MY_SHELL_H
#define MY_SHELL_H

//prototype functions for each that needs user input
void clr(char **args);
void dir(char **args);
void print_environ(char **args);
void quit(char **args);
void cd(char **args);
void redirection(char **args);
void echo(char **args);
void pauce(char **args);
void help(char **args);
void other(char **args);

#endif /*MY_SHELL_H*/