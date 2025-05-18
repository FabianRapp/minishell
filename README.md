# Minishell

A bash 5.2 clone written in C, developed as part of the 42 curriculum. It replicates many features of the real bash, such as parsing, redirection, piping, environment variable handling, and command execution.

---

## 🚀 Features Implemented Like Bash 5.2

when we speak of bash we mean Bash 5.2 (there are surpringly many differneces of behavior between versions)

- **Built-in Commands**: Implements core built-ins with support for various flags:  
  - `cd`, `echo`, `env`, `exit`, `export`, `pwd`, `unset`
- **Environment Management**: Fully manages and updates shell environment variables, including support for:  
  - `$SHLVL`, `env -i`, `export +=`, `$_`
- **Redirection**: Handles complex input/output redirections:  
  - Standard redirection (`>`, `<`, `>>`)
  - **HERE-DOC** (`<<`) and **HERE-STR** (`<<<`)
  - Advanced support for left and right redirection operands (e.g., `2>a`)
- **Piping**: Supports chaining of commands using pipes (`|`).
- **Boolean Operators**: Implements `&&`, `||`, and `;` for command control flow.
- **Wildcard Expansion**: Expands glob patterns (`*`) and integrates with environment variables.
- **Subshell Execution**: Implements **real** bash-like subshells using `()`, including correct redirection handling around subshells.
- **Quoting Support**: Handles multiline content inside:  
  - Double quotes `"..."`, single quotes `'...'`, and round brackets `(...)`
- **Shell State**:  
  - Implements custom `get_pid` logic and supports `$$` expansion, while get_pid was not allowed by the school subject
- **Behavior Cloning**: Meticulously replicates bash quirks and legacy behaviors.

### 🌀 Bash Behaviors and Edge Cases Reproduced

- **Error codes**:
    - error codes for any input we could think of got matched with the error of bash
- **HERE-DOC Quoting**:
  - If the `<<` argument is in double quotes, the content is expanded before being written to the pipe.
- **Partial Parsing Execution**:
  - HERE-DOC content is executed partly during parsing, partly during redirection.
- **Ctrl+D Handling in HERE-DOC**:
  - Emits a warning that includes the total number of lines read so far when prematurely ending HERE-DOC input.
- **Unexpanded Right-Side HERE-DOC**:
  - Argument to `<<` is not expanded (like in bash).
- **Environment and Redirection Errors**:
  - If a variable with whitespace is used as a redirection target without quotes, it’s split and results in an error duo to 'ambiguous redirect'
- **Export Behavior**:
  - Failed `export` does not modify the environment, but variables still appear in `export` environment output when called without arguments.
- **Newline Semantics**:
  - Newline is **not** treated as whitespace (like tab or spae).
- **Subshell Redirection Semantics**:
  - Redirections on the **left** of a subshell are invalid, while redirections on the **right** apply to the entire subshell.

here is a small smple size for subprocess behavior we don't understand why but cloned:

```
bash-5.2$ echo hello world >file

bash-5.2$ (cat && cat) < file
hello world
minishell-$: (cat && cat) <file
hello world

bash-5.2$ <file (cat && cat)
bash: syntax error near unexpected token `('
minishell-$: <file (cat && cat)
minishell: syntax error near unexpected token `('

bash-5.2$ <file cat
hello world
minishell-$: <file cat
hello world

bash-5.2$ (echo a && cat) <file
a
hello world
minishell-$: (echo a && cat) <file
a
hello world

bash-5.2$ (cat && echo a) <file
hello world
a
minishell-$: (cat && echo a) <file
hello world
a
```

.. there is much more of this...

## 📁 Project Structure

```
.
├── builtins/           # Built-in shell command implementations
├── environment/        # Environment variable handling
├── execution/          # Command execution logic, pipes, redirections
├── expansion/          # Variable and wildcard expansion
├── headers/            # Header files for project-wide includes
├── lexer/              # Lexical analysis of input strings
├── libft/              # Custom standard library functions (42 Libft)
├── parser/             # Parsing logic to build ASTs from tokens
├── signals/            # Signal handling (e.g., Ctrl+C)
├── utils/              # Utility functions (memory, errors, I/O, etc.)
├── main.c              # Shell entry point
├── Makefile            # Project build instructions
├── README.md           # Project documentation
```

---

## 🛠️ Compilation

To compile the shell, simply run:

```bash
make
```

This will build the `minishell` executable.

---

## ✅ Usage

Launch the shell by running:

```bash
./minishell
```

Then interact with it as you would with a regular shell:

```bash
minishell-$:  echo Hello, World!
Hello, World!

> ls | grep ".c" > output.txt
```

optionally to modify the prompt/how the shell reports messages:  
make fclean && make SHELL_NAME="I AM SHELL" SHELL_PROMPT="I CAN SPEKAK: "  
./minishell  
I CAN SPEKAK: AAAAAAAA  
I AM SHELL: AAAAAAAA: command not found  
I CAN SPEKAK: 

---

