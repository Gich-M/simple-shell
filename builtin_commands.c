#include "shell.h"

/**
 * exitShell - exits the shell based on arguments provided.
 *
 * @info: pointer to struct CommandInfo.
 *          Contains arguments.
 *
 * Return: exits with a given exit status.
 *         (0) if info->argv[0] != "exit"
 */
int exitShell(CommandInfo *info)
{
	int exitStatus;

	if (info->argv[1])
	{
		exitStatus = strError(info->argv[1]);
		if (exitStatus == -1)
		{
			info->status = 2;
			printError(info, "Invalid number: ");
			ePuts(info->argv[1]);
			ePutchar('\n');
			return (1);
		}
		info->err_num = strError(info->argv[1]);
		return (-2);
	}
	info->err_num = -1;
	return (-2);
}

/**
 * changeDir - Changes the current working directory.
 *
 * @info: pointer to struct CommandInfo.
 *          Contains arguments.
 *
 * Return: Always 0.
 */

int changeDir(CommandInfo *info)
{
	char *current_dir, *target_dir, buffer[1024];
	int res_dir;

	current_dir = getcwd(buffer, 1024);
	if (!current_dir)
		_puts("Error: Unable to retrieve current directory.\n");

	if (!info->argv[1])
	{
		target_dir = getEnvVar(info, "HOME=");
		if (!target_dir)
			res_dir = chdir((target_dir = getEnvVar(info, "PWD=")) ? target_dir : "/");
		else
			res_dir = chdir(target_dir);
	}
	else if (_strcmp(info->argv[1], "-") == 0)
	{
		if (!getEnvVar(info, "OLDPWD="))
		{
			_puts(current_dir);
			_putchar('\n');
			return (1);
		}
		_puts(getEnvVar(info, "OLDPWD=")), _putchar('\n');
		res_dir = chdir((target_dir = getEnvVar(info, "OLDPWD="))
				? target_dir : "/");
	}
	else
		res_dir = chdir(info->argv[1]);

	if (res_dir == -1)
	{
		printError(info, "Failed to change directory. ");
		ePuts(info->argv[1]), ePutchar('\n');
	}
	else
	{
		setEnv(info, "OLDPWD", current_dir);
		setEnv(info, "PWD", getcwd(buffer, 1024));
	}
	return (0);
}


/**
 * myHelp - Display help information for shell commands
 * @info: pointer to struct CommandInfo.
 *          Contains arguments.
 * Return: Always 0
 */
int myHelp(CommandInfo *info)
{
	char **arg_array = info->argv;

	_puts("Custom Shell Help:\n");
	_puts("  ls      - List directory contents\n");
	_puts("  cd      - Change directory\n");
	_puts("  pwd     - Print working directory\n");
	_puts("  cp      - Copy files or directories\n");
	_puts("  mv      - Move (rename) files or directories\n");
	_puts("  rm      - Remove files or directories\n");
	_puts("  mkdir   - Make directory\n");
	_puts("  rmdir   - Remove directory\n");
	_puts("  touch   - Create an empty file\n");
	_puts("  cat     - Concatenate and display file content\n");
	_puts("  echo    - Display a message\n");
	_puts("  man     - Display manual or help information\n");
	_puts("  ps      - Display information about running processes\n");
	_puts("  kill    - Terminate a process\n");
	_puts("  chmod   - Change file permissions\n");
	_puts("  chown   - Change file owner and group\n");
	_puts("  df      - Display disk space usage\n");
	_puts("  du      - Display directory space usage\n");
	_puts("  grep    - Search for a pattern in files\n");
	_puts("  tar     - Create or extract compressed archives\n");
	if (0)
		_puts(*arg_array);
	return (0);
}

