#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  //char* cmd = NULL;

  //REPL Loop
  while(1) {
    printf("$ ");
    fflush(stdout);

    // Wait for user input
    char input[100];
    fgets(input, 100, stdin);

    //TODO: remove newline at end
    input[strlen(input) - 1] = '\0';

    //cmd = strtok(input, " ");

    //exit 0
    if(strncmp(input, "exit 0",strlen("exit 0")) == 0) {
      exit(0);
    }
    else if(strncmp(input, "echo",strlen("echo")) == 0) {
      printf("%s\n", input + 5);
      continue;
    }
    printf("%s: command not found\n", input);
  }

  return 0;
}
