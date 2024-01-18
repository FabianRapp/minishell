#### Minishell

### Structure

The code base is devided into isolated modules where each should have its own tests

There are a few few dirs that are shared:
	- "libft" dir
	- "utils" dir (only the utils in the root directory of this project)
	- "main_headers" dir

## Main Headers
These header files should enable all the functions that are supposed to be usable from any module (for example any libft function).
The interface functions of the modules will be decalared here


## Modules

# lexer (lexical analyzer)
Breaks down user input into tokens, such as commands, special chars, or integers. It simplifies the input by identifying key elements and represnting these with the data type t_token, preparing it for further processing by the parser.

Interfac functions:
	t_lexer		new_lexer(char *str); creates a nex lexer object, needs a NULL terminated string
	t_token		next_new_token(t_lexer *lexer); returns the next token by value

# REPL (“Read Eval Print Loop”)
Includes the main function
Calls the diffrent modules


