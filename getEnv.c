#include "shell.h"

/**
 * getEnviron - retrieves a copy of the environment strings.
 * @info: pointer to struct CommandInfo.
 *          Contains arguments.
 * Return: Always 0.
 */
char **getEnviron(CommandInfo *info)
{
	if (!info->environ || info->env_altered)
	{
		info->environ = listToStrings(info->env);
		info->env_altered = 0;
	}
	return (info->environ);
}

/**
 * unsetEnv - Removes an environment variable.
 * @info: pointer to struct CommandInfo.
 *          Contains arguments.
 * @var: The string environment variable property.
 * Return: 1 on delete, 0 otherwise.
 */
int unsetEnv(CommandInfo *info, char *var)
{
	StringList *node = info->env;
	size_t i = 0;
	char *p;

	if (!node || !var)
		return (0);
	while (node)
	{
		p = startsWith(node->str, var);
		if (p && *p == '=')
		{
			info->env_altered = deleteNodeAtIndex(&(info->env), i);
			i = 0;
			node = info->env;
			continue;
		}
		node = node->next;
		i++;
	}
	return (info->env_altered);
}

/**
 * setEnv - Initializes a new environment variable or modifies an existing one.
 * @info: pointer to struct CommandInfo.
 *          Contains arguments.
 * @var: The string environment variable property.
 * @value: The string env var value.
 * Return: Always 0.
 */
int setEnv(CommandInfo *info, char *var, char *value)
{
	char *buf;
	char *p;
	StringList *node;

	if (!var || !value)
		return (1);
	buf = malloc(_strlen(var) + _strlen(value) + 2);
	if (!buf)
		return (1);
	_strcpy(buf, var);
	_strcat(buf, "=");
	_strcat(buf, value);
	node = info->env;
	while (node)
	{
		p = startsWith(node->str, var);
		if (p && *p == '=')
		{
			free(node->str);
			node->str = buf;
			info->env_altered = 1;
			return (0);
		}
		node = node->next;
	}
	addNodeEnd(&(info->env), buf, 0);
	free(buf);
	info->env_altered = 1;
	return (0);
}
