# Basic C++ Shell

This is a minimum viable product for a basic terminal shell. It is written in C++.

Compiled with C++17 using MSVC.

It is modeled after Bash.

## Features

- **Builtin Commands**. This basic shell supports the following builtins:
    - `exit`: Exit the shell with a specified status code. Ex: `exit 1`.
    - `pwd`: Print the current working directory. Ex: `pwd`.
	- `cd`: Change the current working directory. Ex: `cd ../`, `cd ~`, `cd /d/Documents`.
    - `echo`: Print text to the terminal. Ex: `echo "Hello World"`. 
	Supports typical Bash behavior for single and double quotes, as well as escape sequences. Interpolation not supported.
	- `type`: 
        - If the argument is a builtin, displays that to the screen.
		- If the argument is not a builtin, displays the path to the program, if it exists.
		- If the argument is not a builtin or a program, displays an error message.
- **Other commands**. If the command is not a builtin, it is assumed to be a program and is executed.

## Usage

Compile (with at least C++17) and run the program. A shell will then be started.
Type any of the above commands and any arguments you want.
