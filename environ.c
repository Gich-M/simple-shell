#include "shell.h"

/**
 * myEnv - prints the current environment.
 *
 * @info: pointer to struct CommandInfo.
 *          Contains arguments.
 *
 * Return: Always 0
 */

int myEnv(CommandInfo *info)
{
	printListStr(info->env);
	return (0);
}

/**
 * getEnvVar - gets value of the current environment variable.
 *
 * @info: pointer to struct CommandInfo.
 *          Contains arguments.
 *
 * @name: environment variable name
 *
 * Return: value
 */

char *getEnvVar(CommandInfo *info, const char *name)
{
	StringList *node = info->env;
	char *value;

	while (node)
	{
		value = startsWith(node->str, name);
		if (value && *value)
			return (value);
		node = node->next;
	}
	return (NULL);
}

/**
 * setEnvVar - initializes a new environment variable or
 *                 modifies an existing one.
 *
 * @info: pointer to struct CommandInfo.
 *          Contains arguments.
 *
 * Return: Always 0.
 */

int setEnvVar(CommandInfo *info)
{
	if (info->argc != 3)
	{
		ePuts("Invalid number of arguments\n");
		return (1);
	}
	if (setEnv(info, info->argv[1], info->argv[2]))
	{
		return (0);
	}
	else
	{
		return (1);
	}
}

/**
 * unsetEnvVar - removes an environment variable.
 *
 * @info: pointer to struct CommandInfo.
 *          Contains arguments.
 *
 * Return: Always 0.
 */

int unsetEnvVar(CommandInfo *info)
{
	int i;

	if (info->argc == 1)
	{
		ePuts("Few arguments entered.\n");
		return (1);
	}
	for (i = 1; i <= info->argc; i++)
		unsetEnv(info, info->argv[i]);
	return (0);
}

/**
 * populateEnvList - populates the environment linked list
 *
 * @info: pointer to struct CommandInfo.
 *          Contains arguments.
 *
 * Return: Always 0.
 */
int populateEnvList(CommandInfo *info)
{
	StringList *node = NULL;
	size_t i;

	for (i = 0; environ[i]; i++)
		addNodeEnd(&node, environ[i], 0);
	info->env = node;
	return (0);
}

