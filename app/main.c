#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  char* cmd = NULL;

  //REPL Loop
  while(1) {
    printf("$ ");
    fflush(stdout);

    // Wait for user input
    char input[100];
    fgets(input, 100, stdin);

    //TODO: remove newline at end
    input[strlen(input) - 1] = '\0';

    cmd = strtok(input, " ");
    printf("%s: command not found\n", cmd);
  }
    
  return 0;
}
