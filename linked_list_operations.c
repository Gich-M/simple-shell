#include "shell.h"

/**
 * listLen - determines the length of a linked list
 * @h: Pointer to the first node
 * Return: Size of the list
 */
size_t listLen(const StringList *h)
{
	size_t size = 0;

	while (h)
	{
		h = h->next;
		size++;
	}
	return (size);
}

/**
 * listToStrings - returns an array of strings from the list->str field
 * @head: Pointer to the first node
 * Return: Array of strings
 */
char **listToStrings(StringList *head)
{
	StringList *node = head;
	size_t i = listLen(head);
	char **strs;
	char *str;
	size_t j;

	if (!head || !i)
		return (NULL);
	strs = malloc(sizeof(char *) * (i + 1));
	if (!strs)
		return (NULL);
	for (i = 0; node; node = node->next, i++)
	{
		str = malloc(_strlen(node->str) + 1);
		if (!str)
		{
			for (j = 0; j < i; j++)
				free(strs[j]);
			free(strs);
			return (NULL);
		}
		str = _strcpy(str, node->str);
		strs[i] = str;
	}
	strs[i] = NULL;
	return (strs);
}

/**
 * printList - prints all elements of a CommandInfo linked list
 * @h: Pointer to the first node
 * Return: Size of the list
 */
size_t printList(const StringList *h)
{
	size_t size = 0;

	while (h)
	{
		_puts(convertNumber(h->num, 10, 0));
		_putchar(':');
		_putchar(' ');
		_puts(h->str ? h->str : "(nil)");
		_puts("\n");
		h = h->next;
		size++;
	}
	return (size);
}

/**
 * nodeStartsWith - returns a node whose string starts with a prefix
 * @node: Pointer to the list head
 * @prefix: String to match
 * @c: The next character after the prefix to match
 * Return: Matching node or NULL
 */
StringList *nodeStartsWith(StringList *node, char *prefix, char c)
{
	char *p = NULL;

	while (node)
	{
		p = startsWith(node->str, prefix);
		if (p && ((c == -1) || (*p == c)))
			return (node);
		node = node->next;
	}
	return (NULL);
}

/**
 * getNodeIndex - gets the index of a node
 * @head: pointer to the list head
 * @node: pointer to the node
 * Return: index of the node or -1
 */
ssize_t getNodeIndex(StringList *head, StringList *node)
{
	size_t index = 0;

	while (head)
	{
		if (head == node)
			return (index);
		head = head->next;
		index++;
	}
	return (-1);
}

