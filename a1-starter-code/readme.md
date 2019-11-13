A plain text document, called readme.md, which explains your design choices, lists the system calls you
used to implement each of the required features, and elaborates on how you tested your implementation.
Make sure you cite all sources that contributed to your assignment in this file. You may use a markup
language, such as Markdown, to format this plain text file.


Upon compilation using Makefile, main is executed with a few global variables initialized:

1. A child_process pid_t variable initialized to -1 for the purpose of protecting the exit_implementation kill processes
2. a parent id that keeps track of the main dragonshell process for checking later upon exitting with getpid()
3. a path string that is initialized to /bin path. This is used for executing function implementations in the bin folder, such as ls and find /


In main...
Main first initializes signal handlers to kill child processes on CTRL+C and also handle the CTRL+Z input

Dragonshell now starts:
1. We first display the welcome message and enter a loop that terminates only on CTRL + D
    - If the CTRL + D signal is caught, then we kill all the running processes and exit the program.
    - This also happens upon the user inputting "exit" beside the prompt

2. Get user input from the prompt line and tokenize into individual commands that are stored into a command array
    - We then loop through these individual commands

3. Tokenize the individual commands by spaces to separate commands/program executions, and arguments that could change the given steps to take.....
    - Check for any piping/bg process/ redirect arguments in the given tokenized command vector.
    - handle each case accordingly with global booleans that execute the given command

4. Check the space tokenized command. If there exists a cd, pwd, $PATH, a2path, or exit as the first tokenized string, we execute the built-in library commands.
    - else,  this is where we execute a program such that we can access it within the absolutely specified, or relatively specified path using the predefined path string...
        - if we wish to use a bg process, we skip waiting for the process to finish itself and go straight to the prompt.
        - if there is no '&', we actively execute and wait for the program to finish itself before displaying the prompt again. we can cancel using sigaction for CTRL+Z or CTRL+C.

(We restrict the size of the space tokenized output in order to not execute ">" as a command, which gives an error.)

5. For execution, if there is an absolute path specified, execve takes in the last user input as the program to execute
    - if it is executed as a relative path, we use the predefined path string to find the program and execute. (We always use access() to check if the external command can be executed)

6. Piping implementation not finished.
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
For CD:
- chdir(path) was used to implement the CD function. Testfolders were made to test the use of cd .. and cd <name> in conjunction with pwd to see if cd was working correctly

PWD:
-getcwd() was used to obtain the current directory that the prompt was located. this path was then displayed before the prompt was executed..

$PATH:
- no syscalls were used to implement this function as it prints the current PATH directory string that is defined globally and changed with a2path

a2path:
- a2path uses string concatenation after "a2path $PATH:" to edit the globally defined path string. Tested using $PATH.

exit:
- uses a globally defined child process initalized to -1 and checks if the child process changes at all upon termination. otherwise it just exits. If the child process changes or is initialized to something else, it is killed and the program is stopped. This was tested by calling or CTRL + D and seeing if there was a terminated message present...
- used kill(pid, SIG) to kill child processes and _exit() to terminate the program

signals:
- uses sigaction to be tripped upon CTRL+C and CTRL+Z. the code will then jump to the signal handler and use SIGINT or SIGTSTP to terminate child processes accordingly and display the dragonshell prompt once again.. This was tested using long extensive functions such as sleep 10, or find /, and ps to check if the process was killed or not.

background processes:
- uses a boolean that is tied to an else clause that gets skipped if the boolean is true. the else clause holds the waitpid() that makes the program go straight to the prompt display.
- Uses dup2() and close() syscalls to write an unnecessary propmt print-out to /dev/null in order to display the prompt nicely again. This was tested by running sleep 10 & to see if the prompt displayed along with the pid id before the propmt displays again...

multiple programs:
- tokenized into multiple strings in a vector and then looped individually. tested by cd <name>; pwd.

File redirect:
- Tokenized each command by spaces and searched for the > character. Tested by ls > text.txt. the file was then checked and ran in background process to see if the proper ls output was given.
- uses dup2, close, and open to write/create the output to a file of the user's specification...

Execute External:
- Executes an external command either in background or actively.
- Uses the execve() system call to execute a program in a specific directory (used in bin for ls commands, find / etc)


Piping:
-not applicable..