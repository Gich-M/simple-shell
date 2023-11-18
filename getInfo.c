#include "shell.h"

/**
 * clearInfo - initializes CommandInfo struct with default values.
 *
 * @info: pointer to struct CommandInfo.
 *          Contains arguments.
 */
void clearInfo(CommandInfo *info)
{
	info->arg = NULL;
	info->argv = NULL;
	info->path = NULL;
	info->argc = 0;
}

/**
 * setInfo - initializes struct with provided arguments
 *
 * @info: pointer to struct CommandInfo.
 *          Contains arguments.
 *
 * @av: arguments
 */

void setInfo(CommandInfo *info, char **av)
{
	int i;

	info->fname = av[0];
	if (!info->arg)
		return;
	info->argv = strtow(info->arg, " \t");
	if (!info->argv)
	{
		info->argv = malloc(sizeof(char *) * 2);
		if (info->argv)
		{
			info->argv[0] = _strdup(info->arg);
			info->argv[1] = NULL;
		}
	}

	for (i = 0; info->argv && info->argv[i]; i++)
		;

	info->argc = i;
	replaceAlias(info);
	replaceVars(info);
}

/**
 * freeInfo - frees allocated memory and fields of CommandInfo
 *
 * @info: pointer to struct CommandInfo.
 *          Contains arguments.
 *
 * @fullClean: true if freeing fields, false if otherwise
 */

void freeInfo(CommandInfo *info, int fullClean)
{
	ffree(info->argv);
	info->argv = NULL;
	info->path = NULL;

	if (fullClean)
	{
		if (!info->cmd_buf)
			free(info->arg);
		if (info->env)
			freeList(&(info->env));
		if (info->history)
			freeList(&(info->history));
		if (info->alias)
			freeList(&(info->alias));
		ffree(info->environ);
		info->environ = NULL;
		bfree((void **)info->cmd_buf);
		if (info->input_fd > 2)
			close(info->input_fd);
		_putchar(FLUSH_BUFFER);
	}
}

