#include <stddef.h>
#include <string.h>
#include <stdio.h>
#define MAX_SIZE 405834

/**
 * @brief Tokenize a C string 
 * 
 * @param str - The C string to tokenize 
 * @param delim - The C string containing delimiter character(s) 
 * @param argv - A char* array that will contain the tokenized strings
 * Make sure that you allocate enough space for the array.
 */
void tokenize(char* str, const char* delim, char ** argv) {
  char* token;
  token = strtok(str, delim);
  for(size_t i = 0; token != NULL; ++i){
    argv[i] = token;
  token = strtok(NULL, delim);
  }
}

int main(int argc, char **argv) {
  // print the string prompt without a newline, before beginning to read
  printf("Welcome to dragonshell\n");
  
  char c;
  int buffersize = MAX_SIZE;
  
  char* arr[buffersize];
  char* arr1[buffersize];
  
  char *command_line;
  char **arguments;

  // tokenize the input, run the command(s), and print the result
  const char *delimiter = ';';
  int i = 0;
  while (1){
    printf("Dragonshell > ");
    // scan and store in an array, or get standard input rather
    // for now, assume that input IS LOWER THAN MAX ARRAY SIZE THAT YOU INITIALIZE
    c = getchar();
    if (c == EOF || c == '\n' ){
      arr[i] = '\0';
      break;
    } else{
      arr[i] = c;
    }
    i++;
    if (i >= buf0fersize){
      // increase the maximum size that the array can hold, not sure how to do this
    }
  }
  // tokenize the command prompt input strings and store into arr2 which contains each tokenized string
  tokenize(arr,delimiter, arr1);

  // call independent functions for each library command

  // take the tokenized array and evaluate for certain commands.\

  //cd (could do this all with ifs)

      //read the first two characters by passing the tokenized array. commence cd command when the first two characters is equal to cd..

  //pwd

  //....

  // print output


  return 0;
}



