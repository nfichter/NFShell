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
	char * buffer = calloc(1024,sizeof(char));
  	fgets(buffer, 1024, stdin);
	int i = 0;
	while (buffer[i] != 0) {
		if (buffer[i] == '\n') {
			buffer[i] = 0;
		} else {
			i++;
		}
	}
	strncpy(retBuff,buffer,1024);
	free(buffer);
}

//Splits the buffer by semicolons into strings to be split by spaces
char ** splitBySemicolon(char * buffer) {
	char ** commandsSplitBySemicolon;
	char * p = buffer;
	int i = 0;
	while (p) {
		commandsSplitBySemicolon[i] = strsep(&p,";");
		i++;
	}
	commandsSplitBySemicolon[i] = 0;
	
	return commandsSplitBySemicolon;
}

//Splits one command's string by spaces
char ** splitBySpace(char * buffer) {
	char **commandsSplitBySpace = (char **)calloc(20,sizeof(char[1024]));
  	int i = 0;
  	char * p = strdup(buffer);
  	while (p) {
  		commandsSplitBySpace[i] = (char *)calloc(64,sizeof(char));
		commandsSplitBySpace[i] = strsep(&p," ");
		if (strcmp(commandsSplitBySpace[i],"\n") == 0) {
	  		commandsSplitBySpace[i] = 0;
		}
		i++;
  	}
  	commandsSplitBySpace[i] = 0;
  	
  	return commandsSplitBySpace;
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

void run() {
	printPrompt();
	char buffer[1024];
	char *p = buffer;
	getBuffer(p);
	int i = 0;
	char **s = splitBySemicolon(p);
	while (s[i]) {
		char *s2 = s[i];
		char **commands = splitBySpace(s2);
		exec1(commands);
		wait(0);
		i++;
	}
}

int main() {
  	while (1) {
  		run();
  	}
}
