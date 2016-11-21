#include <stdlib.h>
#include <stdio.h>

#include <string.h>
#include <unistd.h>

#include <errno.h>

int isExit(char *command) {
  if (strcmp(command,"exit") == 0) {
      return 1;
  }
  return 0;
}

int isLs(char *command) {
  if (strcmp(command,"ls") == 0) {
      return 1;
  }
  return 0;
}

void printPrompt() {
  char username[1024];
  getlogin_r(username, 1024);
  char hostname[1024];
  gethostname(hostname, 1024);
  char cwd[1024];
  getcwd(cwd,1024);
  
  printf("%s@%s:%s$ ",username,hostname,cwd);
}

void exec() {
  char buffer[100];
  while (fgets(buffer, 100, stdin) == NULL) {}
  
  char *s = buffer;
  char *command[100];
  
  int i = 0;
  while (buffer[i] != '\n' && buffer[i] != 0) {
    i++;
  }
  buffer[i] = 0;

  i = 0;
  while (s) {
    command[i] = strsep(&s," ");
    if (strcmp(command[i],"\n") == 0) {
      command[i] = 0;
    }
    i++;
  }
  command[i] = 0;
  int f = fork();
  if (f == 0) {
    printPrompt();
    int err = execvp(command[0], command);
    if (err == -1) {
      printf("Error: %d, %s\n",errno,strerror(errno));
    }
    exit(0);
  }
}

int main() {
  int exit = 0;
  while (exit == 0) {
    printPrompt();
    exec();
  }
}
