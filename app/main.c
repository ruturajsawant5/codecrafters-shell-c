#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

const int num_my_shell_builtins = 3;
char my_shell_builtins[3][10] = {"exit", "echo", "type"};

int main() {
  char* cmd = NULL;
  char* dup_input = NULL;
  char* arg = NULL;
  char* path = NULL;
  char* path_token = NULL;
  char* dup_path = NULL;
  int i;
  int flag;
  char exec_path[256];
  pid_t pid;
  char** args;
  int nargs = 0;

  path =  getenv("PATH");

  //REPL Read Evaluate Print Loop
  while(1) {
    printf("$ ");
    fflush(stdout);

    // Wait for user input
    char input[100];
    fgets(input, 100, stdin);

    //remove newline at end
    //input[strlen(input) - 1] = '\0';
    input[strcspn(input, "\n")] = 0;

    //duplicate string
    dup_input = strdup(input);
    cmd = strtok(dup_input, " ");
    arg = strtok(NULL, "");

    //exit 0
    if(strncmp(input, "exit 0",strlen("exit 0")) == 0) {
      exit(0);
    }
    else if(strncmp(input, "echo",strlen("echo")) == 0) {
      printf("%s\n", input + 5);
      continue;
    }
    else if(strncmp(input, "type",strlen("type")) == 0) {
      flag = 0;

      //arg = strtok(NULL, " ");
      for(i = 0; i < num_my_shell_builtins; i++) {
        if(strncmp(arg, my_shell_builtins[i], strlen(my_shell_builtins[i])) == 0) {
          printf("%s is a shell builtin\n", arg);
          flag = 1;
          break;
        }
      }
      if(!flag) {
        dup_path = strdup(path);
        path_token = strtok(dup_path, ":");
        while(path_token) {
          sprintf(exec_path, "%s/%s", path_token, arg);
          
          if(access(exec_path, X_OK) == 0) {
            flag = 1;
            printf("%s is %s\n", arg, exec_path);
            break;
          }
          path_token = strtok(NULL, ":");
        }
      }
      if(!flag)
        printf("%s: not found\n", arg);
      continue;
    }
    else {
      flag = 0;
      int cmd_fnd = 0;

      dup_path = strdup(path);
      path_token = strtok(dup_path, ":");
      while(path_token) {
        sprintf(exec_path, "%s/%s", path_token, cmd);
        
        if(access(exec_path, X_OK) == 0) {
          cmd_fnd = 1;
          //printf("%s is %s\n", arg, exec_path);
          break;
        }
        path_token = strtok(NULL, ":");
      }

      // while(cmd) {
      //   args = realloc(args, ++nargs * sizeof(char*));
      //   args[nargs - 1] = malloc(strlen(cmd) + 1);
      //   strcpy(args[nargs - 1], cmd);
      //   //printf("%s ---- \n", args[nargs - 1]);
      //   cmd = strtok(NULL, " ");
      // }
      // args[nargs] = NULL;

      // dup_path = strdup(path);
      // path_token = strtok(dup_path, ":");
      //while(path_token) {
        // sprintf(exec_path, "%s/%s", path_token, args[0]);
        // printf("exec_path=%s\n", exec_path);
        // if(access(exec_path, X_OK) == 0) {



        if(cmd_fnd){

          char* jk[3];
          int x=0;
          jk[x++] = strdup(cmd);
          jk[x++] = strdup(arg);
          jk[x++] = NULL;
          // printf("H->%s\n", jk[x-1]);
          // char* arg_tk = strtok(arg, " ");
          // while(arg_tk) {
          //   jk[x++] = strdup(arg_tk);
          //   arg_tk = strtok(NULL, " ");
          //   printf("H->%s\n", jk[x-1]);
          // }



          pid = fork();

           

          if(pid == 0) {
            //child  
            execvp(cmd, jk);
          }
          else {
            //parent aka shell
            wait(NULL);
            flag = 1;
            //printf("\n");
            //free(args);
          }

          //break;
        //}
       // path_token = strtok(NULL, ":");
      //}

        }
      if(!cmd_fnd)
        printf("%s: command not found\n", cmd);
    }
  }

  return 0;
}
