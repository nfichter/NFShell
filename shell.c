#include <stdlib.h>
#include <stdio.h>

#include <string.h>
#include <unistd.h>

#include <errno.h>

//Prints the prompt
void printPrompt() {
  char username[1024];
  getlogin_r(username, 1024);
  char hostname[1024];
  gethostname(hostname, 1024);
  char cwd[1024];
  getcwd(cwd,1024);
  
  printf("%s@%s:%s$ ",username,hostname,cwd);
}

//Gets input, copies it to the given buffer
void getBuffer(char * retBuff) {
	char * buffer = calloc(100,sizeof(char));
  	while (fgets(buffer, 100, stdin) == NULL) {}
	int i = 0;
	while (buffer[i] != '\n' && buffer[i] != 0) {
		i++;
	}
	buffer[i] = 0;
	strncpy(retBuff,buffer,1023);
	free(buffer);
}

//Splits the buffer by semicolons into strings to be split by spaces
char ** splitBySemicolon(char * buffer) {
	return 0;
}

//Splits one command's string by spaces
char ** splitBySpace(char * buffer) {
	char **command;
  	int i = 0;
  	while (buffer) {
		command[i] = strsep(&buffer," ");
		if (strcmp(command[i],"\n") == 0) {
	  		command[i] = 0;
		}
		i++;
  	}
  	command[i] = 0;
  	
  	return command;
}

//Helper function that checks if the function is exit
int isExit(char *command) {
  char *p;
  p = "exit";
  if (strcmp(command,p) == 0) {
	  return 1;
  }
  return 0;
}

//Helper function that checks if the function is cd
int isCd(char *command) {
  char *p;
  p = "cd";
  if (strcmp(command,p) == 0) {
	return 1;
  }
  return 0;
}

//Executes one command (including flags)
void exec1(char ** command) {
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
  while (1) {
	printPrompt();
	char * buffer = calloc(1024,sizeof(char));
	getBuffer(buffer);
	exec1(splitBySpace(buffer));
  }
}
