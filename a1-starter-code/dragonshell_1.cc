#include <vector>
#include <string>
#include <cstring>
#include <iostream>

#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <cstdio>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <signal.h>
#include <fcntl.h>

#define EXIT_SUCCESS 0

pid_t childpid = -1;
pid_t dragonshell_process = getpid();

std:: string PATH = "/bin/:/usr/bin/";                                                  // initialized path variable. used in execution and $PATH commands for changing, also gets tokenized by colons


void cd_implementation(std:: string s);                                                 // function prototypes
void pwd_implementation();
void $PATH_implementation();
void a2path_implementation(std::vector<std::string> s);
void exit_implementation();
int execute_cmd(std::vector<std::string> tokens,std::string s);
void pipe_init();


/**
 * @brief Tokenize a string 
 * 
 * @param str - The string to 
 * @param delim - The string containing delimiter character(s)
 * @return std::vector<std::string> - The list of tokenized strings. Can be empty
 */
std::vector<std::string> tokenize(const std::string &str, const char *delim) {
  char* cstr = new char[str.size() + 1];
  std::strcpy(cstr, str.c_str());

  char* tokenized_string = strtok(cstr, delim);

  std::vector<std::string> tokens;
  while (tokenized_string != NULL)
  {
    tokens.push_back(std::string(tokenized_string));
    tokenized_string = strtok(NULL, delim);
  }
  delete[] cstr;

  return tokens;
}

void sigint_handler(int signal){                      // handle SIGINT signal upon sigaction command
  if (getpid() != dragonshell_process){
    kill(getpid(),SIGINT);                            // kill
  }
}
void sigstp_handler(int signal){
  if (getpid() != dragonshell_process){               // handle sigtstp signal upon sigaction command
    //sigtstp handler
    kill(getpid(),SIGTSTP);                           // kill
  }
}

int main(int argc, char **argv) {
  // print the string prompt without a newline, before beginning to read
  // tokenize the input, run the command(s), and print the result
  // do this in a loop
  // dragonshell has initiated

  struct sigaction sa;                                 // set sigaction to jump to sigint handler on ^C input
  sa.sa_flags = SA_RESTART;                            // flag
  sigemptyset(&sa.sa_mask);
  sa.sa_handler = sigint_handler;
  sigaction(SIGINT, &sa, NULL);                        // jump


  struct sigaction st;                                  // set sigaction to jump to sigtstp on ^Z input
  st.sa_flags = SA_RESTART;                             //flag
  sigemptyset(&st.sa_mask);
  st.sa_handler = sigstp_handler;
  sigaction(SIGTSTP,&st,NULL);                          // jump



  std::cout << "Welcome to dragonshell. \n";            // startup
  while (std::cin){                                     // std:: cin executes exit command upon ^D input to kill processes 
    std::cout << "Dragonshell > ";                      // display prompt

    //declare delimeter and input for tokenize to use
    std:: string delim = ";";                           // initialize delimiters to tokenize commands into vectors
    std:: string space_delim = " ";
    std:: string input;                                 // initialized user input to tokenize
    std:: string file_redirect;                         // catches the user output file upon output redirection
    std:: string file_input_stream;                     // catches user input function upon output redirection
    unsigned int idx;                                   // catches index of the tokenized > in redirect
    unsigned int bgidx;                                 // catches the index of tokenized & in bg processes. important for redirection of output

    //get input and store inside input variable
    getline(std::cin, input);                           // prompt input
    std::vector<std::string> tokenizer = tokenize(input, delim.c_str());          // tokenize with ;

    for (unsigned int i = 0;i < tokenizer.size(); i++){                           // loop through the separated commands
      bool backgroundprocess_bool = false;                                        // booleans to go true only if the symbols &,>, or | are present
      bool piping_bool = false;
      bool outputredirect = false;
      bool reserved = false;
      // get the string and assign to a variable
      std:: string s = tokenizer.at(i);                                                           // s is equal to the given user command
      std:: vector<std::string> tokenizer_output_addon = tokenize(s, space_delim.c_str());        // take s and tokenize by spaces to separate arguments and command
      idx = tokenizer_output_addon.size();                                                        // reset the tokenizer_output_addons to the correct size
      bgidx = tokenizer_output_addon.size();
      for (unsigned int i = 0; i < tokenizer_output_addon.size(); i++){                           // loop linearly through the " " tokenized individual commands to find |, &, or >
        
        if (tokenizer_output_addon.at(i) == "|"){
          piping_bool = true;                                                                     // piping is true if | is found
          // call piping implementation here or.....
        }
        if (tokenizer_output_addon.at(i) == ">"){                                                 // output redirection should be performed if > is present
          outputredirect = true;                                                                  // boolean is true to initialize redirect support
          file_redirect = tokenizer_output_addon.at(i+1);                                         // the name of the output file is stored (after >)
          file_input_stream = tokenizer_output_addon.at(i-1);                                     // the name of the input function is stored (before >)
          reserved = true;                                                                        // boolean to check if the input is a library function
          idx = i;                                                                                // idx stores the index of >
        }
        if (tokenizer_output_addon.at(i) == "&"){                                                 // check for &
          backgroundprocess_bool = true;                                                          // allows execution of bg processes within child processes

          bgidx = i;                                                                              // stores the index of & for editting the tokenized vector later....
        }
      }
      // if a pipe is present, you skip the evaluation of the current command, ie the loop below
      if (piping_bool == true){                                                                   // if we are executing a piping process, we skip the current iterating of the given cmd
        

        continue; // skip the entire bool below
      }

      for (unsigned int j = 0;j < tokenizer_output_addon.size();j++){                             // loop individual command vectors that are tokenized by " "
        if ((tokenizer_output_addon.at(j) == "cd") && (reserved == false)){                       // if the cd is present and we are not redirecting, execute cd into directory as normal
          // execute cd command
          cd_implementation(s);
          break;
          // pass s into cd command
        }
        else if ((tokenizer_output_addon.at(j) == "pwd") && (reserved == false)){                 // if pwd is present in the vector string, and we are not redirecting, execute pwd and print on terminal
          // execute pwd command
          pwd_implementation();
          break;
          // should an error be returned if pwd is included with other different characters included?
        }
        else if ((tokenizer_output_addon.at(j) == "$PATH") && (reserved == false)){               // if $PATH is present in the vector string and we are not redirecting, execute $PATH and print on terminal

          // execute a2path command
          $PATH_implementation();
          break;
        }

        else if ((tokenizer_output_addon.at(j) == "a2path") && (reserved == false)){              // if a2path is present and we are not redirecting, execute...
          // execute a2path command
          a2path_implementation(tokenizer_output_addon);
          break;
        }
        // implement a CTRL + D keyboard command
        // implementation for exit
        else if ((tokenizer_output_addon.at(j) == "exit")){                                       // execute exit command
          // execute exit command
          exit_implementation();
          break;
        }
        else if (tokenizer_output_addon.at(j) == ">"){                                            // if there is a redirect, we do not wish to execute the > as a command, so we skip through the loop
          break;
        }
        else {
          pid_t pid = fork();                                                                     // if none of the build-in commands trip, we create a child process such that the program can be run
          std:: string colon = ":";
          char path[8000];
          getcwd(path, sizeof(path));                                                             //we obtain the bin directory to execute a given command by default, this is initialized in path global string
          if (pid == 0){                                                                          // if we successfully fork,

            if (backgroundprocess_bool== true){
              int fd = open("/dev/null", O_RDONLY);                                               // if there is a bg process, we print the additional "dragonshell >\n" into a null file so the additional dragonshell > prompt comes up properly
              dup2(fd,1);                                                                        
              dup2(fd,2);
              close(fd);
            }
            if (outputredirect == true){
              int file_desc = open(file_redirect.c_str(), O_CREAT | O_WRONLY,S_IRUSR|S_IWUSR);    // if there is a redirect we create and write, but also allow the user to read and write to the given file
              dup2(file_desc, 1);
              close(file_desc);
            }

            if (piping_bool == true){

            }


            int limit =std::  min(idx,bgidx);                                                     // between > and &, the limit of the size of the copying vector becomes the minimum of the two indeces

            std:: vector<std::string> copy = tokenizer_output_addon;
            // tokenize the input string with space
                // get the tokarr[j]
            copy.resize(limit);                                                                   // copy the tokenized by " " string and resize it
            if (tokenizer_output_addon[j].at(0) == '/'){                                          // if the user specifies complete path of a program, execute such that it exists.
              // passes the argument test
              std:: string arg = tokenizer_output_addon[j];
              if (access(arg.c_str(),X_OK) == 0){
                // call execution 
          
                execute_cmd(copy,arg);                                                            // execute and wait for the child process to execute and finish
                wait(NULL);
                return 0;
              }
            }                            // check if the access of the arg string exists
          else{
            std:: string commp =std:: string(path) + "/" + tokenizer_output_addon[j];             // if not specified, we concatenate the user command to the path string (/bin/ls for example), and execute on that entire path.
            if (access(commp.c_str(),X_OK) == 0){                                                 // if the complete path is successfully specified, we execute....
              // call execution function on commp
          
              execute_cmd(copy,commp);                                                            // execute with the pathway initialized in commp
              wait(NULL);                                                                         // wait for child process to execute
              return 0;
            }
            std:: vector<std::string> pathway = tokenize(PATH,colon.c_str());                     // split the predefined path string by its colons and store into the vector
            for (std:: string str : pathway){                                                     // within the pathway vector, we take the user command concatenate with the " " split command tokens
              commp = str + tokenizer_output_addon[j];                                            
              if (access(commp.c_str(),X_OK)==0){                                                 // we check if the program can be executed
                // call execution function on commp
            
                execute_cmd(copy,commp);                                                          // execute
                wait(NULL);
                return 0;
              }
            }
          }
          return 0;
          }if (backgroundprocess_bool){                                                           // if the background process is evaluated, we skip the action of waiting for the active program to execute as we wish to run hte parent process while the child process executes
            childpid = pid;

            std:: cout << "PID " << pid << " in bg\n";                                            // debug

          }else{
        
            waitpid(pid,NULL,WUNTRACED);                                                          // wait for the active process to execute before displaying prompt again
          }
        }
        break;                                                                                    // exit loop and redisplay prompt.
      }
    }

  }
  std:: cout << "\n";                                                                             // upon ^D, the exit implementation is executed
  exit_implementation();


  return 0;
}






// implement cd
void cd_implementation(std:: string s){
  
    std:: string space = " ";                                                       // delimeter to mark splitting is space to split cd command and directory

    std::vector<std::string> tokenizer = tokenize(s, space.c_str());                // tokenize vector contains the cd command in one index and the directory in the other
    
    if (tokenizer.size() >= 2){                                                     // run cd command only if it has an argument
      // the index below is out of range
      std:: string directory = tokenizer.at(1);                                     // get the directory input

      if (directory == ""){                                                         // if no directory, display error message
        std:: cout << "expected argument to the right of cd \n";                    // error message
      }
      const char* path = directory.c_str();                                         // proper argument cast for chdir system command

      if (chdir(path) != 0){                                                        // if the path doesn't exist
        // display error message
        std:: cout << "path does not exist.. \n";

      }
    }
    else{
      std:: cout << "expected argument the rest code which is after the exit() function.o cd \n";                                 // if there is no argument to cd
    }
                                                                                    // wait for child operations to end

}

// implement pwd
void pwd_implementation(){
  //get cwd, handle if pathway is equal to null
                                                                                  // fork
  char buffer[80000];                                                             // initialize buffer array for getcwd
                                                                                  // if successful 
  if (getcwd(buffer,sizeof(buffer)) != NULL){                                     // if the current working directory exists
    //print working directory
    std:: cout << buffer << '\n';
  }
  else{                                                                           // if the current working directory does not exist
    //display error message
    std:: cout << "current path does not exist \n";
  }                                                                               // wait for child processes to end
  
}

// implement $PATH
void $PATH_implementation(){
  std:: cout << "Current PATH: ";                                                 // upon $PATH execution, print the global path string
  
  std:: cout << PATH;
  std:: cout << '\n';
}

void a2path_implementation(std:: vector<std::string> s){
  // tokenize the a2path string
  std:: string colon = ":";                                                       //tokenize the input string by colons 
  std:: string $path = s.at(1);                                                     
  std:: vector<std::string> append = tokenize($path,colon.c_str()); 
  PATH = PATH + colon + append.at(1);                                             // add a colon to the global string and append the user tokenized string after :
} 
// implement exit  

void exit_implementation(){
  std:: cout << "exiting\n";                                                        // does not terminate all processes
  if (childpid != -1){
    // Process exists.
    kill(childpid, SIGTERM); 
  }
  _exit(0);
                                                               // terminate all child processes
}

int execute_cmd(std::vector<std::string> tokens,std::string s){
  // keep, cite the source for proper execve format
  std::vector<char *> charvec;                                 // initialize charvector that stores the  strings in the token array
  charvec.reserve(tokens.size() + 1);                          // make the char vector be of the same size of the command token vector
  for (std:: string str : tokens)                              // iterate through the tokenized " " split user input
      charvec.push_back(strdup(str.c_str()));                  // append the strings through the space split user input as char const * into the vector to pass
  charvec.push_back(NULL);                                     // null terminate char const * vector

  // for (char* dick: charvec){
  //   std::cout << dick << "\n";
  // }
  
  if (execve(s.c_str(),charvec.data(),NULL) == -1){            // if the execution does not work, we display the type of error we get when trying to execute the program in the path defined directory
    std::cout << "Error: " << errno << "\n";
  }

  for (char * s1: charvec){                                    // after the process is done, we free all the memory assigned to the charvector array
    free(s1);
  }
  return 0;                                                    // go back to main...
}

void pipe_init(){
  // deal with piping
  // check for file redirection within..
  
}
