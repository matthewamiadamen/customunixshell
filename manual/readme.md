CA216 Shell User Manual
By Matthew Amiadamen

 Introduction
Thank you for taking the time to read my shell user manual this manual is designed to give an introductory level of understanding and an overview of how shell commands, work and to help users understand unix operating systems. Essentially what a shell program is is an interface between the kernel(the backend of the computer) and the user.

A terminal can allow you to edit and move around your files, work with I/0 redirection, the different environment variables of a process aswell as how to run multiple programs at the same time(background program execution)

Basic Usage
To use the shell, simply open a terminal window or command prompt. You'll see a command prompt, which typically ends with a dollar sign ($) or another symbol depending on your configuration. This prompt indicates that the shell is ready to accept commands.

To first use a shell you open up a terminal window this is the basic access interface you will use the majority of the time, from there you type into a command prompt that usually ends in a dollar sign.

Before the dollar sign you’ll notice a sequence of forward slash separated words, this is the file path to your current directory and essentially shows where in the computer you are. To find out more information you type commands into the terminal and you press enter to execute them, for example pressing ls will show you what files and contents are in your current folder which is very user friendly for getting your bearings of the system.

 I/O Redirection
I/O redirection is a powerful feature of the shell that allows you to control where input comes from and where output goes. You can redirect input from a file, and output to a file or another program.
Input output redirection is a feature of the shell that allows for easy saveability of your program execution.

To redirect te output of your program to a file the > symbol is used after the program name and then is followed by the file you wish to output to for input the same goes but with > symbol.
When you specify the output file normally your shell is connected to the stdin and stdout so it appears there but in this case these are changed through functions to the actual files, if the > character is used if the output file does not exist it is created and if it does it replaces what was there but if you use >> it actually knows to append to what was already there.

 Program Environment
The shell provides an environment for running programs. This environment includes variables, paths, and settings that affect how programs behave.
As I previously mentioned the command prompt specifies the path taken by your computer to get to the current directory, this actually comes from something called environment variables.

Whenever a process is running in your computer, this is just any kind of program weather it be user activated or integral to the functioning of the computer, it has a job id a process id aswell as environment variables which are just traits of information about said programme that affects how it behaves.

 Aswell as the path variable which is a list of directories where in this case the shell looks for executable programmes there is also the home variable which shows you your current home directory and pwd for ones current working directory.

Background Program Execution
You can run programs in the background by appending an ampersand (&) to the command. This allows you to continue using the shell while the program runs.
Initially you might assume most programs are ran in the foreground meaning its one of the only process running at that time depending on how many cores your machine has and you must wait for it to finish computing to make another command but in this shell you also have access to background program execution.
This means that by appending an ampersand (&) to the end of your command line arguments the shell will still be fully available to you whilst your chosen programme executes. For example a sleep programme with & would run till completion in the background.

Broad Explanation of Shell Code
The shell code I have provided implements a basic shell with several intresting features that come in 3 major categories. Here is an overview of the main components and functions.

 **`main()`**:
The main function serves as the basis for the code I this is where standard input is taken and where I the commands have been mapped to their relevant functions, a loop then ensues where the user can continuously enter commands through the buf.

Another way the user can interact with this shell is through a batchfile, similar to io redirection a batchfile is a line by line file of commands that can be used as stdin for the shell instead of user input, to activate this simply enter ./shell followed by the batchfile you would like to use, the system will figure out the rest.

- **Built-in Commands**:
In terms of the actual command we use the second major component are the built in command you can use, these are clr, dir, environ, quit, cd echo and pause.

Pause essentially stops the function of the shell until the user presses the enter button it does this by changing the settings of the terminal to not show what is being types and only register new line characters as input (Disabling echoing and canonical mode). 

Some of the other built in internal functions call upon system calls directly from the kernel rather than through shell programming. For example the clr command simply calls on the system call “clear” to clear the screen.

An important Note is that when using cd command there is no need for "" the shell will pick up the directory without it being written as a string literal

- **I/O Redirection**: The `redirection` function as previously mentioned handles I/O redirection by forking a child process which basically works in congruence with a parent process unless stated otherwise and and redirects standard input or output to files specified by the user.

For Programmes outside the standard inbuilt ones the shell works with external commands by forking into a child process, finding the file path of the executable file and then using this along with the arguments as parameters for the execvp function which runs said programme in the computer.

This shell  overall helps provides a basic command line  interface similar to other UNIX shells, allowing users to execute commands, manage files, and control program execution.

## References
- Kernighan, B. W., & Pike, R. (1984). The UNIX Programming Environment. Prentice Hall.
Linux Documentation Project. (n.d.). Advanced Bash-Scripting Guide. Retrieved from http://tldp.org/LDP/abs/html/index.html
GNU Project. (n.d.). Bash Reference Manual. Retrieved from https://www.gnu.org/software/bash/manual/bash.html
Linuxize. (n.d.). Understanding Shell Commands and Writing Shell Scripts. Retrieved from https://linuxize.com/post/bash-scripting-cheat-sheet/
Bash Academy. (n.d.). Learn Bash in Y Minutes. Retrieved from https://learnxinyminutes.com/docs/bash/
Linux Journal. (n.d.). Bash by Example, Part 1. Retrieved from https://www.linuxjournal.com/content/bash-example-part-1

My name is Matthew Amiadamen student number 22505553 23/13/23 and I would like to acknowledge dcus academic integrity policy and my full understanding that this is all my work.
