#include <stdlib.h>
#include <stdio.h>

#include <string.h>
#include <unistd.h>

#include <errno.h>

int isExit(char *command) {
  char *p;
  p = "exit";
  if (strcmp(command,p) == 0) {
      return 1;
  }
  return 0;
}

int isCd(char *command) {
  char *p;
  p = "cd";
  if (strcmp(command,p) == 0) {
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

  if (isExit(command[0])) {
    exit(0);
  }

  else if (isCd(command[0])) {
    if (command[1]) {
      int err = chdir(command[1]);
      if (err == -1) {
      	printf("Error: %d, %s\n",errno,strerror(errno));
      }
    }
    else {
      chdir("cd");
    }
  }

  else {
  	int f = fork();
  	if (f == 0) {
    	int err = execvp(command[0], command);
    	if (err == -1) {
      		printf("Error: %d, %s\n",errno,strerror(errno));
    	}
    	exit(0);
  	}
  	else {
    	wait(NULL);
  	}
  }	
}
int main() {
  int exit = 0;
  while (exit == 0) {
    printPrompt();
    exec();
  }
}
