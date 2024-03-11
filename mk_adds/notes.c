WHAT MINISHELL HAS TO HANDLE:

- The shell will work only in interactive mode (no scripts, i.e. the executable takes no arguments)
- Run simple commands with absolute, relative path (e.g. /bin/ls, ../bin/ls)
- Run simple commands without a path (e.g. ls, cat, grep, etc…)
- Have a working history (you can navigate through commands with up/down arrows)
- Implement pipes (|)
- Implement redirections (<, >, >>)
- Implement the here-doc (<<)
- Handle double quotes ("") and single quotes (''), which should escape special characters, beside $ for double quotes.
- Handle environment variables ($ followed by a sequence of characters).
- Handle signals like in bash (ctrl + C, ctrl + \, ctrl + D).
	- Ctrl + C: 
		• When I press Ctrl+C, it sends the SIGINT (signal interrupt) signal to the currently running process.
		• This signal is typically used to interrupt or terminate the currently running command or script.
	- ctrl + \:
		• When I press Ctrl+\, it sends the SIGQUIT signal to the running process.
		• This signal is similar to SIGINT, but it also generates a core dump of the process, 
			providing more information about its state at the time of termination.
		• It is often used to forcefully terminate a process and obtain debugging information.
	- ctrl + D:
		• It signals an "end-of-file" (EOF).
		• At the beginning of an empty line, it indicates that there is no more input, and the shell will exit.
		• At the end of a line with input, it will act as a regular character (nothing will happen visibly)
- Implement the following built-ins:
	— echo [option -n only (echo alone prints always a new line at the end, with the -n it skips the new line character)] 
	— exit
	— env (with no options or arguments)
	— export (with no options)
	— unset (with no options)
	— cd
	— pwd
	And for the bonus part
	— handle && and || with the parenthesis () for priority.
	— handle * wildcards for the current working directory.



GENERAL ARCHITECTURE:
_____________________

1. "front-end" part: it deals with user input and user interaction, like commands and signals
	Practically two things to handle:
		- a command (user input as a  line/string)
		- signals //i should leave t for the end
2. "back-end" part: the internal work, else execution.


PARSING THE INPUT:
1) The "lexical analysis" or "tokenization":
	- takes the input from the user, proccesses it char by char and splits it into 'tokens'. (tokens have a type and a value.)
2) the "syntax analysis" or "parsing":
	- we scan the stream of tokens according to a specific grammar and then decide what to do with them.
		e.g. generate an AST - Abstract Syntax Tree.




ADDITIONAL NOTES:
___________________

- The program should be linked with the readline library when compiling. For example, gcc your_program.c -o your_program //! -lreadline


EDGE CASES:
-------------
• cat | cat | ls : bash seems to print first the output of the ls in the current directory, and right after it waits for input, like 
					what cat does alone. //! WHY???
• cat /dev/urandom : it is an endless file, minishell shouldn not crash


