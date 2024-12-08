#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

const int num_my_shell_builtins = 5;
char my_shell_builtins[5][10] = {"exit", "echo", "type", "pwd", "chdir"};

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
  int rc;
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
    else if (strncmp(input, "cd", strlen("cd")) == 0) {
      getcwd(buffer, 255);
      if(strncmp(arg, "../", strlen("../")) == 0) {
        
        char* last_slash = strrchr(buffer, "/");
        if(last_slash) {
          size_t length = last_slash - buffer;
          char new_path[256];
          strncpy(new_path, buffer, length);
          new_path[length] = 0;
          rc = chdir(new_path);
          if(rc != 0)
            printf("cd: %s: No such file or directory\n", new_path);
        }
      }
      else if(strncmp(arg, "./", strlen("./")) == 0) {
        char new_path[256];
        sprintf(new_path, "%s/%s", buffer, arg+2);
        rc = chdir(new_path);
        if(rc != 0)
          printf("cd: %s: No such file or directory\n", new_path);

      }
      rc = chdir(arg);
      if(rc != 0)
        printf("cd: %s: No such file or directory\n", arg);
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
