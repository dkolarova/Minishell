🧠 Minishell (42 Project)
📌 Overview

This project is an implementation of a Unix shell in C, inspired by Bash.

It allows execution of commands, handling of pipes, redirections, and built-in commands, while managing processes and signals.

Main challenges

Parsing and tokenizing user input correctly

Handling pipes and redirections efficiently

Managing multiple processes safely

Handling signals (Ctrl+C, Ctrl+D)

Preventing memory leaks and ensuring clean termination

📄 minishell.h

The minishell.h header defines all core data structures, shared resources, and function prototypes used in the project.
It centralizes shell state, command representation, and helper utilities.

📊 Global Shell Structure (t_shell)
typedef struct s_shell
{
    char    **env;           // Environment variables
    char    *input;          // Current command input
    int     exit_status;     // Last command exit code
}   t_shell;
🔍 Explanation

t_shell stores the global state of the shell:

env → environment variables for command execution

input → user-entered command line

exit_status → last executed command status

All shared variables are accessed carefully to avoid inconsistencies.

🧩 Functional Modules
🔹 Input Handling
char *read_input(void);

Reads and sanitizes input from the user

Handles EOF (Ctrl+D) and empty input

🔹 Parsing (parser)
int tokenize_input(t_shell *shell);
int parse_command(char *input, t_command **cmd);

Splits input into tokens (commands, arguments, operators)

Builds internal representation for execution

Handles quotes, variable expansion, and special characters

🔹 Execution (executor)
int execute_command(t_shell *shell, t_command *cmd);

Executes built-in commands internally (cd, echo, pwd, etc.)

Launches external commands using fork() and execve()

Manages pipes and file descriptor redirections

🔹 Built-ins (builtins)
int builtin_cd(t_shell *shell, char **args);
int builtin_echo(t_shell *shell, char **args);
int builtin_exit(t_shell *shell, char **args);

Implements core built-in commands

Updates shell state accordingly

🔹 Pipes and Redirections (redirection)
int handle_pipes(t_command *cmd);
int handle_redirections(t_command *cmd);

Connects commands with pipes

Redirects input/output safely

Ensures no file descriptor leaks

🔹 Signal Handling (signals)
void setup_signals(void);
void handle_signal(int sig);

Handles Ctrl+C, Ctrl+D, and Ctrl+\

Ensures the shell does not terminate unexpectedly

Resets input state after interruptions

🔹 Utilities (utils)
void free_command(t_command *cmd);
char *get_env_var(t_shell *shell, const char *name);

Helper functions for memory management, environment handling, and command utilities

🔹 Cleanup (cleanup)
void free_shell(t_shell *shell);

Frees all allocated memory

Prevents leaks and undefined behavior

🧠 Design Summary

One shell process → main program

External commands → child processes

Pipes → connected using pipe() system calls

Redirections → managed with file descriptors

Signals → handled to maintain interactive shell behavior

Clean initialization and cleanup of all resources

Created by ~Diana Kolarova
