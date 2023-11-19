#include "shell.h"

/**
 * displayHistory - displays command history with line numbers.
 *
 * @info: pointer to struct CommandInfo.
 *          Contains arguments.
 *
 * Return: Always 0.
 *
 */

int displayHistory(CommandInfo *info)
{
	printList(info->history);
	return (0);
}

/**
 * unsetAlias - Unsets an alias by removing it from the alias list.
 *
 * @info: pointer to struct CommandInfo.
 *          Contains arguments.
 *
 * @aliasStr: The string representing the alias.
 *
 * Return: 0 on success, 1 on error.
 */

int unsetAlias(CommandInfo *info, char *aliasStr)
{
	char *eqPos;
	char savedChar;
	int result;

	eqPos = _strchr(aliasStr, '=');
	if (!eqPos)
		return (1);

	savedChar = *eqPos;
	*eqPos = 0;
	result = deleteNodeAtIndex(&(info->alias),
			getNodeIndex(info->alias, nodeStartsWith(info->alias, aliasStr, -1)));
	*eqPos = savedChar;
	return (result);
}

/**
 * setAlias - Sets alias to a string.
 *
 * @info: pointer to struct CommandInfo.
 *          Contains arguments.
 *
 * @aliasStr: The string representing the alias.
 *
 * Return: 0 on success, 1 on error.
 */
int setAlias(CommandInfo *info, char *aliasStr)
{
	char *equalPos;

	equalPos = _strchr(aliasStr, '=');

	if (!equalPos)
		return (1);

	if (!*++equalPos)
		return (unsetAlias(info, aliasStr));
	unsetAlias(info, aliasStr);
	return (addNodeEnd(&(info->alias), aliasStr, 0) == NULL);
}

/**
 * printAlias - Prints an alias string.
 *
 * @aliasNode: The alias node containing the alias string.
 *
 * Return: 0 on success, 1 on error.
 */
int printAlias(StringList *aliasNode)
{
	char *equalPos = NULL, *aliasStr = NULL;

	if (aliasNode)
	{
		equalPos = _strchr(aliasNode->str, '=');
		for (aliasStr = aliasNode->str; aliasStr <= equalPos; aliasStr++)
			_putchar(*aliasStr);
		_putchar('\'');
		_puts(equalPos + 1);
		_puts("'\n");
		return (0);
	}
	return (1);
}

/**
 * myAlias - Replicates functionality of the alias built-in command.
 *
 * @info: pointer to struct CommandInfo.
 *          Contains arguments.
 *
 * Return: Always 0.
 */
int myAlias(CommandInfo *info)
{
	int i = 0;
	char *equalPos = NULL;
	StringList *node = NULL;

	if (info->argc == 1)
	{
		node = info->alias;
		while (node)
		{
			printAlias(node);
			node = node->next;
		}
		return (0);
	}
	for (i = 1; info->argv[i]; i++)
	{
		equalPos = _strchr(info->argv[i], '=');
		if (equalPos)
		{
			setAlias(info, info->argv[i]);
		}
		else
		{
			printAlias(nodeStartsWith(info->alias, info->argv[i], '='));
		}
	}
	return (0);
}

