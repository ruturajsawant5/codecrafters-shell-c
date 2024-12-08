#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

const int num_my_shell_builtins = 4;
char my_shell_builtins[4][10] = {"exit", "echo", "type", "pwd"};

int main() {
  char *cmd = NULL;
  char *arg = NULL;
  char *path = NULL;
  char *path_token = NULL;
  char *dup_path = NULL;
  int i;
  int flag;
  char exec_path[256];
  char buffer[256];
  pid_t pid;
  char **args;
  int nargs = 0;
  char *token;

  path = getenv("PATH");

  // REPL Read Evaluate Print Loop
  while (1) {
    printf("$ ");
    fflush(stdout);

    // Wait for user input
    char input[100];
    fgets(input, 100, stdin);

    // remove newline at end
    // input[strlen(input) - 1] = '\0';
    input[strcspn(input, "\n")] = 0;

    // duplicate string
    // dup_input = strdup(input);
    cmd = strtok(input, " ");
    arg = strtok(NULL, "");

    // exit 0
    if (strncmp(cmd, "exit", strlen("exit")) == 0) {
      exit(0);
    } else if (strncmp(cmd, "echo", strlen("echo")) == 0) {
      printf("%s\n", arg);
      continue;
    } 
    else if (strncmp(input, "pwd", strlen("pwd")) == 0) {
      getcwd(buffer, 255);
      printf("%s\n", buffer);
      continue;
    }
    else if (strncmp(input, "type", strlen("type")) == 0) {
      flag = 0;

      for (i = 0; i < num_my_shell_builtins; i++) {
        if (strncmp(arg, my_shell_builtins[i], strlen(my_shell_builtins[i])) ==
            0) {
          printf("%s is a shell builtin\n", arg);
          flag = 1;
          break;
        }
      }
      if (!flag) {
        dup_path = strdup(path);
        path_token = strtok(dup_path, ":");
        while (path_token) {
          sprintf(exec_path, "%s/%s", path_token, arg);

          if (access(exec_path, X_OK) == 0) {
            flag = 1;
            printf("%s is %s\n", arg, exec_path);
            break;
          }
          path_token = strtok(NULL, ":");
        }
      }
      if (!flag)
        printf("%s: not found\n", arg);
      continue;
    } else {
      flag = 0;

      dup_path = strdup(path);
      path_token = strtok(dup_path, ":");
      while (path_token) {
        sprintf(exec_path, "%s/%s", path_token, cmd);

        if (access(exec_path, X_OK) == 0) {
          flag = 1;
          break;
        }
        path_token = strtok(NULL, ":");
      }

      if (flag) {

        nargs = 0;
        args = malloc((nargs + 1) * sizeof(char *));
        args[nargs++] = strdup(cmd);

        token = strtok(arg, " ");
        while (token) {
          args = realloc(args, (nargs + 1) * sizeof(char *));
          args[nargs++] = strdup(token);
          token = strtok(NULL, " ");
        }
        args = realloc(args, (nargs + 1) * sizeof(char *));
        args[nargs++] = NULL;

        pid = fork();

        if (pid == 0) {
          // child
          execvp(cmd, args);
        } else {
          // parent aka shell
          wait(NULL);
          flag = 1;
        }
      }
      if (!flag)
        printf("%s: command not found\n", cmd);
    }
  }

  return 0;
}
