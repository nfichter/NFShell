# NFShell

by Noah Fichter

Features:
	-Executes single commands ignoring excess whitespace
	-Executes multiple commands separated by semicolons ignoring excess whitespace
	-Implements simple redirection using < and >
	----Can only be used once per single command (can be used multiple times if you use a semicolon)
	----< can only be used if it is the first argument in a command
	-Implements simple piping using |
	----Only one pipe may be used
		
Bugs:
	-Only one < or > may be used, same with pipes
	
Functions:
	-void printPrompt();
	Inputs: None
	Returns: None
	Prints the prompt
	
	-void getBuffer(char * retBuff); 
	Inputs: char * retBuff
	Returns: None
	Copies the text the user enters into retBuff
	
	-char ** splitBySemicolon(char * buffer);
	Inputs: char * buffer
	Returns: A double pointer of full commands split by semicolon
	Splits the full command into an array of commands split by semicolon
	
	-char ** splitBySpace(char * buffer);
	Inputs: char * buffer
	Returns: A double pointer of full commands split by semicolon
	Splits a single command into an array of sub-commands split by space
	
	-int count(char * str, char toCount);
	Inputs: char * str, char toCount
	Returns: The number of occurrences of toCount in str
	Counts the number of occurrences of toCount in str
	
	-int pipeCheck(char *command);
	Inputs: char *command
	Returns: The number of occurrences of "|" in command
	Counts the number of occurrences of "|" in command
	
	-int redirectCheck(char *command);
	Inputs: char *command
	Returns:
	---- -1 if there is only one occurrence of "<" in command
	---- 1 if there is only one occurrence of ">" in command
	---- 0 if there are no occurrences of either "<" or ">" in command
	---- 2 if there are 2 or more occurrences of a combination of ">" and "<" in command
	Counts the number of occurrences of "<" and ">" in command
	
	-int isExit(char *command);
	Inputs: char *command
	Returns: 1 if the command is "exit", 0 otherwise
	Checks if the command is exit (special command)
	
	-int isCd(char *command);
	Inputs: char *command
	Returns: 1 if the command is "cd", 0 otherwise
	Checks if the command is cd (special command)
	
	-void exec1(char ** fullCommand); 
	Inputs: char **fullCommand
	Returns: None
	Executes one full command (i.e. multiple words separated by spaces)
	
	-void piper(char **commands, char *s2);
	Inputs: char **commands, char *s2
	Returns: None
	Executes two commands by piping the output of the first to the input of the second
	
	-void redir(char *s2);
	Inputs: char *s2
	Returns: None
	Executes a full command, deals with redirection, calls piper()
	
	-void run(); 
	Inputs: None
	Returns: None
	Prints the prompt, gets the buffer, and calls redir() to actually run the buffer
	
	-int main();
	Inputs: None
	Returns: None
	Repeatedly calls run