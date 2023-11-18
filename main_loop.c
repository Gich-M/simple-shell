#include "shell.h"

/**
 * hsh - main shell loop
 * @info: the parameter & return info struct
 * @av: the argument vector from main()
 *
 * Return: 0 on success, 1 on error, or error code
 */
int hsh(CommandInfo *info, char **av)
{
	ssize_t size = 0;
	int builtin_ret = 0;

	while (size != -1 && builtin_ret != -2)
	{
		clearInfo(info);
		if (isInteractive(info))
			_puts("Custom_Shell$ ");
		_putchar(FLUSH_BUFFER);
		size = getInput(info);
		if (size != -1)
		{
			setInfo(info, av);
			builtin_ret = findBuiltin(info);
			if (builtin_ret == -1)
				findCmd(info);
		}
		else if (isInteractive(info))
			_putchar('\n');
		freeInfo(info, 0);
	}
	writeHistory(info);
	freeInfo(info, 1);
	if (!isInteractive(info) && info->status)
		exit(info->status);
	if (builtin_ret == -2)
	{
		if (info->err_num == -1)
			exit(info->status);
		exit(info->err_num);
	}
	return (builtin_ret);
}

/**
 * findBuiltin - finds a builtin command
 * @info: the parameter & return info struct
 *
 * Return: -1 if builtin not found,
 *			0 if builtin executed successfully,
 *			1 if builtin found but not successful,
 *			-2 if builtin signals exit()
 */
int findBuiltin(CommandInfo *info)
{
	int i, builtIn_ret = -1;
	BuiltinCommandTable builtintbl[] = {{"exit", exitShell},
		{"env", myEnv},
		{"help", myHelp},
		{"history", displayHistory},
		{"setenv", setEnvVar},
		{"unsetenv", unsetEnvVar},
		{"cd", changeDir},
		{"alias", myAlias},
		{NULL, NULL}};

	for (i = 0; builtintbl[i].type; i++)
		if (_strcmp(info->argv[0], builtintbl[i].type) == 0)
		{
			info->line_count++;
			builtIn_ret = builtintbl[i].func(info);
			break;
		}
	return (builtIn_ret);
}

/**
 * findCmd - finds a command in PATH
 * @info: the parameter & return info struct
 *
 * Return: void
 */
void findCmd(CommandInfo *info)
{
	char *path = NULL;
	int i, k;

	info->path = info->argv[0];
	if (info->linecount_flag == 1)
	{
		info->line_count++;
		info->linecount_flag = 0;
	}
	for (i = 0, k = 0; info->arg[i]; i++)
		if (!isDelim(info->arg[i], " \t\n"))
			k++;
	if (!k)
		return;

	path = findPath(info, getEnvVar(info, "PATH="), info->argv[0]);
	if (path)
	{
		info->path = path;
		forkCmd(info);
	}
	else
	{
		if ((isInteractive(info) || getEnvVar(info, "PATH=") ||
					info->argv[0][0] == '/') &&
				isCmd(info, info->argv[0]))
			forkCmd(info);
		else if (*(info->arg) != '\n')
		{
			info->status = 127;
			printErrorInfo(info, "not found\n");
		}
	}
}

/**
 * forkCmd - forks a an exec thread to run cmd
 * @info: the parameter & return info struct
 *
 * Return: void
 */
void forkCmd(CommandInfo *info)
{
	pid_t child_pid = fork();

	if (child_pid == -1)
	{
		perror("Error forking process");
		exit(EXIT_FAILURE);
	}
	if (child_pid == 0)
	{
		if (execve(info->path, info->argv, getEnviron(info)) == -1)
		{
			freeInfo(info, 1);
			if (errno == EACCES)
			{
				fprintf(stderr, "Error: Permission denied.\
						Unable to execute '%s'\n", info->path);
				exit(126);
			}
			exit(1);
		}
	}
	else
	{
		int wait_status = wait(&(info->status));

		if (wait_status == -1)
		{
			perror("Error waiting for child process");
			exit(EXIT_FAILURE);
		}
		handleWaitStatus(info, wait_status);
	}
}

/**
 * handleWaitStatus - Handle the exit status of a child process
 * @info: Pointer to struct CommandInfo containing process information.
 * @wait_status: Status returned by the wait system call.
 */
void handleWaitStatus(CommandInfo *info, int wait_status)
{
	if (WIFEXITED(wait_status))
	{
		info->status = WEXITSTATUS(wait_status);

		if (info->status == 126)
		{
			printErrorInfo(info, "Permission denied");
		}
		else if (info->status == 127)
		{
			printErrorInfo(info, "Command not found");
		}
	}
	else
	{
		fprintf(stderr, "Child process did not exit normally\n");
		exit(EXIT_FAILURE);
	}
}
