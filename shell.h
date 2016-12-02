void printPrompt(); //prints the prompt
void getBuffer(char * retBuff); //copies the buffer into the given string pointer
char ** splitBySemicolon(char * buffer); //splits the buffer string into an array of strings by semicolon,
										 //each string representing a full command (returns the array of strings)
char ** splitBySpace(char * buffer); //splits each command string from ^^^ into another array of strings,
								     //each string representing a single command (returns the array of strings)
int count(char * str, char toCount); //counts the occurrences of the character toCount in the string str, returns the number
int pipeCheck(char *command); //checks for the number of pipes in a full command, returns number of pipes found
int redirectCheck(char *command); //checks for number of redirection symbols ("<" and ">"), returns:
										//-1 if there is only one occurrence of <
										//1 if there is only one occurrence of >
										//0 if there are no occurrences of either
										//2+ if there are too many of either
int isExit(char *command); //checks if the command is exit, returns 0 if not, 1 if so
int isCd(char *command); //checks if the command is cd, returns 0 if not, 1 if so
void exec1(char ** command); //executes one full command separated by spaces
void piper(char **commands, char *s2); //side run function, checks for piping and does it
void redir(char *s2); //bulk of run function, checks for redirection and does it
void run(); //main running function
int main(); //just calls run() repeatedly