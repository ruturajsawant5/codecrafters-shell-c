#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int num_my_shell_builtins = 3;
char my_shell_builtins[3][10] = {"exit", "echo", "type"};

int main() {
  char* cmd = NULL;
  char* dup_input = NULL;
  char* arg = NULL;
  int i;

  //REPL Loop
  while(1) {
    printf("$ ");
    fflush(stdout);

    // Wait for user input
    char input[100];
    fgets(input, 100, stdin);

    //TODO: remove newline at end
    input[strlen(input) - 1] = '\0';

    //duplicate string
    dup_input = strdup(input);
    cmd = strtok(dup_input, " ");

    //exit 0
    if(strncmp(input, "exit 0",strlen("exit 0")) == 0) {
      exit(0);
    }
    else if(strncmp(input, "echo",strlen("echo")) == 0) {
      printf("%s\n", input + 5);
      continue;
    }
    else if(strncmp(input, "type",strlen("type")) == 0) {
      arg = strtok(NULL, " ");
      for(i = 0; i < num_my_shell_builtins; i++) {
        if(strncmp(arg, my_shell_builtins[i], strlen(my_shell_builtins[i])) == 0) {
          printf("%s is a shell builtin\n", arg);
          break;
        }
      }
      if(i == num_my_shell_builtins)
        printf("%s: not found\n", arg);
      continue;
    }
    printf("%s: command not found\n", cmd);
  }

  return 0;
}
