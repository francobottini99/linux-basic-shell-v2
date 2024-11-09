# MyShell - Extended Command Line Interpreter

This project extends the [MyShell](https://github.com/francobottini99/LINUXSHELL1-2022.git) command line interpreter. The main goal is to expand the functionalities of [MyShell](https://github.com/francobottini99/LINUXSHELL1-2022.git) by adding signal handling, pipes, and input/output redirection.

### Author:
- **Franco Nicolas Bottini**

## Main Features

### 1. Internal Commands
MyShell continues to support basic internal commands such as `cd`, `clr`, `echo`, `quit`, and introduces new ones.

- **cd \<directory\>**: Changes the current directory to \<directory\>. The `cd -` option returns to the last working directory.

- **clr**: Clears the screen.

- **echo \<comment\|env var\>**: Displays \<comment\> on the screen followed by a newline.

- **quit**: Exits MyShell.

- **jobs**: Lists all running jobs.

- **kill \<job id\>**: Terminates a job or process specified by its ID.

- **fg \<job id\>**: Resumes a specified job in the foreground.

### 2. Signal Handling
Signal handling for CTRL-C, CTRL-Z, and CTRL-\ has been implemented. These signals are sent to the foreground job instead of MyShell. If no foreground job is running, no action is taken.

### 3. Pipes
MyShell now supports piping using the **|** operator, which connects the standard output of one process to the standard input of the next.

Examples:
```
$ last <username> | wc -l
$ ps aux | grep firefox
$ grep bash /etc/passwd | cut -d “:” -f 1 | sort -r
```

### 4. I/O Redirection
MyShell handles input/output redirection using the `<` and `>` operators. For example:

```
program arg1 arg2 < inputfile > outputfile
```

This executes `program` with `arg1` and `arg2`, using `inputfile` as the standard input and `outputfile` as the standard output. Redirection also works for the internal `echo` command.

### 5. External Commands
MyShell supports the execution of external commands. Programs located in the file system can be run using relative or absolute paths.

### 6. Background Execution
MyShell still allows background execution of commands using the `&` operator at the end of a command line. When a job is launched in the background, a message indicating the job ID and process ID is printed.

Example:
```
$ echo 'hello' &
[1] 10506
hello
```

## Compilation and Execution

To compile the project, run:

```
make
```

To execute MyShell, use:

```
./myshell [batchfile]
```

- If a batchfile is provided as an argument, MyShell will execute the commands from the file and exit when the end of the file is reached.
- If no argument is provided, MyShell will display a prompt and wait for user commands via stdin.
