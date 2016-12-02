#include <stdlib.h>
#include <stdio.h>

#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include <errno.h>

#include "shell.h"

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

//Helper function to check for the number of characters in a string
int count(char * str, char toCount) {
	int i = 0;
	int counter = 0;
	while (str[i]) {
		if (str[i] == toCount) {
			counter++;
		}
		i++;
    }
    return counter;
}

//Checks for pipes ("|"), returns number of pipes found (2+ will cause error)
int pipeCheck(char *command) {
	return count(command,'|');
}

//Checks for redirection, returns -1 if <, 1 if >, 0 if neither, and 2+ if too many (will cause error)
int redirectCheck(char *command) {
	int sum = count(command,'<') + count(command,'>');
	if (sum == 0 || sum > 1) {
		return sum;
	}
	if (sum == 1) {
		if (count(command,'<') == 1) {
			return -1;
		} else {
			return 1;
		}
	}
	return 0;
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

//Helper function to get stuff out of run()
void piper(char **commands, char *s2) {
	if (pipeCheck(s2) > 1) {
		printf("Too many | characters. Try again.\n");
	} else if (pipeCheck(s2) == 0) {
		exec1(commands);
	} else {
		
	}
}

void redir(char *s2) {
	if (redirectCheck(s2) > 1) { //case 1: too much redirection
		printf("Too many > or < characters. Try again.\n");
	} else if (redirectCheck(s2) == 0) { //case 2: no redirection
		char **commands = splitBySpace(s2);
		piper(commands,s2);
	} else { //case 3: redirection
		if (redirectCheck(s2) == -1) { // < redirection
			char **commands = splitBySpace(s2);
			if (strcmp(commands[0],"<") == 0) {
				int fd = open(commands[1],O_RDWR|O_CREAT,0774);
				if (fd == -1) {
					printf("Error: %d, %s\n",errno,strerror(errno));
				}
				int stdinNew = dup(0);
				dup2(fd,0);
				run();
				dup2(stdinNew,0);
			} else {
				printf("You may not put arguments before a redirection of stdin.");
			}
		} else { // > redirection
			char **commands = splitBySpace(s2);
			int j = 0;
			int redirPos;
			while (commands[j]) {
				if (strcmp(commands[j],">") == 0) {
					redirPos = j;
					break;
				}
				j++;
			}
			int fd = open(commands[redirPos+1],O_RDWR|O_CREAT,0774);
			if (fd == -1) {
				printf("Error: %d, %s\n",errno,strerror(errno));
			}
			int stdoutNew = dup(1);
			dup2(fd,1);
			commands[redirPos] = 0;
			exec1(commands);
			dup2(stdoutNew,1);
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
		redir(s2);
		i++;
	}
}

int main() {
  	while (1) {
  		run();
  	}
}
