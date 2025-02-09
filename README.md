# Minishell

This is a project about creating a simple shell. MiniShell will introduce you to the world of shells, which provide a convenient text interface to interact with your system. Shells might seem very easy to understand but have very specific and defined behaviour in almost every single case, most of which will need to be handled properly.

## Introduction
The existence of shells is linked to the very existence of IT. At the time, all developers agreed that communicating with a computer using aligned 1/0 switches was seriously irritating. It was only logical that they came up with the idea of creating a software to communicate with a computer using interactive lines of commands in a language somewhat close to the human language. Thanks to Minishell, you’ll be able to travel through time and come back to problems people faced when Windows didn’t exist.

## About the project
The behavior of the minishell is heavily based on the bash behavior. For every point, take bash as a reference. It is implemented with the rules described below.


Implement the following ___builtins___:
- `echo` and the `-n` option
- `cd` only with an absolute or relative path
- `pwd` without any options
- `export` without any options
- `unset` without any options
- `env` without any options or arguments
- `exit` without any options

Implement the following ___functions___:
- Display a __prompt__ while waiting for a new order.
- Have a working __history__.
- Find and launch the correct executable (based on a PATH environment variable or using an absolute path).
- Handle `’` __(single quote)__ which should prevent the shell from interpreting the metacharacters in the quoted sequence.
- Handle `"` __(double quote)__ which should prevent the shell from interpreting the metacharacters in the quoted sequence except for $ (dollar sign).
- `’` and `"` must work as in bash, except multiline.
- Implement __redirections__:
    - `<` should redirect input.
    - `>` should redirect output.
    - `<<` should be given a delimiter, then read the input until a line containing the delimiter is seen. However, it doesn’t have to update the history!
    - `>>` should redirect output in append mode.
- Implement __pipes__ (| character). The output of each command in the pipeline is connected to the input of the next command via a pipe.
- Handle __environment variables__  ($ followed by a sequence of characters) which should expand to their values.
- Handle `$?` variable which should expand to the exit status of the most recently executed foreground pipeline.
- Handle `ctrl-C`, `ctrl-D` and `ctrl-\` signals which should have the same behavior as in bash:
    - `ctrl-C` displays a new prompt on a new line.
    - `ctrl-D` exits the shell.
    - `ctrl-\` does nothing.
- Wildcards * should work for the current working directory.

## Installation
> ⚠️ **Warning**: Don't copy/paste code you don't understand: it's bad for you, and for the school.

If you don't have gcc or GNU make do this commands:
``` sh
sudo apt update
sudo apt install build-essential
```
The program work both on MacOS and on Linux. <br>
For Linux you need to run this additional command:
``` sh
sudo apt-get install lib32readline8 lib32readline-dev
```
The rest of the commands are the same for both MacOS and Linux (Ubuntu)
___
First of all for this project you need to install GNU readline library. For that do this command:
```sh
wget ftp://ftp.gnu.org/gnu/readline/readline-8.2.tar.gz
```
(Or you can download readline-8.2.tar.gz from [__HERE__](https://ftp.gnu.org/gnu/readline/)) <br>
After that do this commands:
``` sh
tar xvfz readline-8.2.tar.gz
cd readline-8.2
./configure --prefix=/$HOME/readline
make
make install
cd ..
rm -rf readline-8.2 readline-8.2.tar.gz
```


Now, to use this project you need to clone this repository, by running the following command:
``` sh
git clone https://github.com/hel-asli/minishell.git
```
Then you should go to the cloned repo and run the make command:
``` sh
cd minishell
make
```
After that you will see the executable minishell. Lastly you should run the executable:
```sh
./minishell
```
You will be provided with a prompt and can use it as a shell.


## Credit
This two-person project was done by both [Hamza El Asli](https://github.com/hel-asli) and [Ouadie El Fengour](https://github.com/Wayde-Ouadie).
