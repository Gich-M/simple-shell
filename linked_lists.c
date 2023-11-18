#include "shell.h"

/**
 * addNode - Adds a node to the start of the list.
 *
 * @head: Address of pointer to head node.
 *
 * @str: Str field of node.
 *
 * @num: Node index used by history.
 *
 * Return: Pointer to the new head.
 */
StringList *addNode(StringList **head, const char *str, int num)
{
	StringList *newHead;

	if (!head)
		return (NULL);
	newHead = malloc(sizeof(StringList));
	if (!newHead)
		return (NULL);

	_memset((void *)newHead, 0, sizeof(StringList));
	newHead->num = num;

	if (str)
	{
		newHead->str = _strdup(str);
		if (!newHead->str)
		{
			free(newHead);
			return (NULL);
		}
	}
	newHead->next = *head;
	*head = newHead;
	return (newHead);
}

/**
 * addNodeEnd - Adds a node to the end of the list.
 * @head: Address of pointer to head node.
 * @str: Str field of node.
 * @num: Node index used by history.
 * Return: Pointer to the new node.
 */
StringList *addNodeEnd(StringList **head, const char *str, int num)
{
	StringList *newNode, *node;

	if (!head)
		return (NULL);
	node = *(head);
	newNode = malloc(sizeof(StringList));

	if (!newNode)
		return (NULL);
	_memset((void *)newNode, 0, sizeof(StringList));
	newNode->num = num;

	if (str)
	{
		newNode->str = _strdup(str);
		if (!newNode->str)
		{
			free(newNode);
			return (NULL);
		}
	}
	if (node)
	{
		while (node->next)
			node = node->next;
		node->next = newNode;
	}
	else
	{
		*head = newNode;
	}
	return (newNode);
}

/**
 * printListStr - Prints only the str element of a list_t linked list.
 * @h: Pointer to the first node.
 * Return: Size of the list.
 */
size_t printListStr(const StringList *h)
{
	size_t size = 0;

	while (h)
	{
		_puts(h->str ? h->str : "(nil)");
		_puts("\n");
		h = h->next;
		size++;
	}
	return (size);
}

/**
 * deleteNodeAtIndex - Deletes node at the given index.
 *
 * @head: Address of pointer to the first node.
 *
 * @index: Index of the node to delete.
 *
 * Return: 1 on success, 0 on failure.
 */
int deleteNodeAtIndex(StringList **head, unsigned int index)
{
	StringList *node, *prevNode;
	unsigned int i = 0;

	if (!head || !*head)
		return (0);
	if (!index)
	{
		node = *head;
		*head = (*head)->next;
		free(node->str);
		free(node);
		return (1);
	}
	node = *head;

	while (node)
	{
		if (i == index)
		{
			prevNode->next = node->next;
			free(node->str);
			free(node);
			return (1);
		}
		i++;
		prevNode = node;
		node = node->next;
	}
	return (0);
}

/**
 * freeList - Frees all nodes of a list.
 *
 * @headPtr: Address of pointer to the head node.
 *
 * Return: void.
 */
void freeList(StringList **headPtr)
{
	StringList *node, *nextNode, *head;

	if (!headPtr || !*headPtr)
		return;
	head = *headPtr;
	node = head;

	while (node)
	{
		nextNode = node->next;
		free(node->str);
		free(node);
		node = nextNode;
	}
	*headPtr = NULL;
}

