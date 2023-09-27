# Lite-Shell

My-Shell is a robust Unix shell implemented in the C programming language, designed to provide users with a powerful and user-friendly command-line interface. It offers a wide array of essential features to enhance your command-line experience. With My-Shell, you can effortlessly perform fundamental tasks such as navigating directories, listing file contents, and executing commands available within your system's PATH environment variable. Additionally, the shell boasts an extensive command history functionality, enabling users to effortlessly access and review their previous commands by simply typing 'history'.

## Features

- **Simple Command Execution:** Users can enter basic Unix commands without the need to identify complex commands involving pipes or input/output redirection. However, the shell supports passing arguments to commands.

- **Foreground Execution:** All commands run in the foreground, which means that a command is executed by a child process, and the parent process waits for the child process to complete before accepting the next command.

- **Command History:** Typing the 'history' command in the shell displays a list of all commands entered during the program's execution. The most recently executed command is always included in the history, and it also includes the PID of the process that executed the command.

- **Built-in Commands:** My-Shell includes several built-in commands that are implemented independently by the shell and do not create child processes. These built-in commands are:
  - `cd`: This command allows users to change directories using the `chdir` function. The shell handles errors gracefully, treating failures as system command failures.
  - `exit`: Typing this command in the shell will terminate the program and exit the shell.

## Usage

When you change environment variables, My-Shell appends information to the environment variable rather than overwriting it. This ensures that all other environment variables passed to the program at runtime remain unchanged. You can verify this by running the `env` command in the shell to see all original environment variables along with any changes you made to specific environment variables.

Please note that using `cd` within My-Shell will not affect the original shell environment after the program's execution.

## Requirements

This shell project meets the following requirements:

- Users can input simple Unix commands.
- All commands run in the foreground.
- The 'history' command displays a list of all entered commands, including the PID of the executing process.
- Three built-in commands are implemented: 'history,' 'cd,' and 'exit.'

## How To Run
To clone and run this application, you'll need Git installed on your computer.

From your command line:

**Clone this repository.**

git clone https://github.com/guyreuveni33/Lite-Shell

**Go into the repository.**

cd Lite-Shell

**Compile.**

gcc -o liteShell.out liteShell.c

**Run the program.**

./liteShell.out

**Exit the program.**

exit

## Running Example

<img src="https://i.postimg.cc/0yRfWk2x/image.png" width="300" height="800">

<img src="https://i.postimg.cc/0yRfWk2x/image.png" width="300" height="800">

For further details about error handling and additional functionalities, please refer to the project's source code and documentation.

---

*This project was developed as part of a programming assignment in Operation System course.*
