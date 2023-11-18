#include "shell.h"

/**
 * getHistoryFile - Gets the history file path
 *
 * @info: pointer to struct CommandInfo.
 *          Contains arguments.
 *
 * Return: Allocated string containing the history file path.
 */
char *getHistoryFile(CommandInfo *info)
{	char *homeDir;
	char *filePath;

	homeDir = getEnvVar(info, "HOME=");

	if (!homeDir)
		return (NULL);

	filePath =
		malloc(sizeof(char) * (_strlen(homeDir) + _strlen(HISTORY_FILE) + 2));
	if (!filePath)
		return (NULL);

	filePath[0] = 0;
	_strcpy(filePath, homeDir);
	_strcat(filePath, "/");
	_strcat(filePath, HISTORY_FILE);
	return (filePath);
}

/**
 * writeHistory - creates a file, or writes to an existing file
 *
 * @info: pointer to struct CommandInfo.
 *          Contains arguments.
 *
 * Return: 1 on success, -1 on failure.
 */
int writeHistory(CommandInfo *info)
{
	ssize_t fd;
	char *filename = getHistoryFile(info);
	StringList *node = info->history;

	if (!filename)
		return (-1);
	fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
	free(filename);
	if (fd == -1)
		return (-1);
	for (node = info->history; node; node = node->next)
	{
		printStringToFD(node->str, fd);
		writeCharToFD('\n', fd);
	}
	writeCharToFD(FLUSH_BUFFER, fd);
	close(fd);
	return (1);
}

/**
 * readHistory - Reads history from a file.
 *
 *  @info: pointer to struct CommandInfo.
 *          Contains arguments.
 *
 * Return: Histcount on success, 0 otherwise.
 */
int readHistory(CommandInfo *info)
{
	int i, last = 0, linecount = 0;
	ssize_t fd, readLength, fileSize = 0;
	struct stat st;
	char *buffer = NULL, *filename = getHistoryFile(info);

	if (!filename)
		return (0);
	fd = open(filename, O_RDONLY);
	free(filename);
	if (fd == -1)
		return (0);
	if (!fstat(fd, &st))
		fileSize = st.st_size;
	if (fileSize < 2)
		return (0);
	buffer = malloc(sizeof(char) * (fileSize + 1));
		if (!buffer)
			return (0);
	readLength = read(fd, buffer, fileSize);
	buffer[fileSize] = 0;
	if (readLength <= 0)
		return (free(buffer), 0);
	close(fd);
	for (i = 0; i < fileSize; i++)
	{
		if (buffer[i] == '\n')
		{
			buffer[i] = 0;
			buildHistoryList(info, buffer + last, linecount++);
			last = i + 1;
		}
	}
	if (last != i)
		buildHistoryList(info, buffer + last, linecount++);
	free(buffer);
	info->histcount = linecount;
	while (info->histcount-- >= HISTORY_MAX)
		deleteNodeAtIndex(&(info->history), 0);
	renumberHistory(info);
	return (info->histcount);
}

/**
 * buildHistoryList - Adds an entry to the history linked list.
 *
 * @info: pointer to struct CommandInfo.
 *          Contains arguments.
 *
 * @buffer: Buffer.
 *
 * @linecount: History linecount.
 *
 * Return: Always returns 0.
 */
int buildHistoryList(CommandInfo *info, char *buffer, int linecount)
{
	StringList *node = info->history;

	if (info->history)
		node = info->history;
	addNodeEnd(&node, buffer, linecount);
	if (!info->history)
		info->history = node;
	return (0);
}

/**
 * renumberHistory - renumbers the history linked list after changes.
 *
 * @info: pointer to struct CommandInfo.
 *          Contains arguments.
 *
 * Return: The new histcount.
 */
int renumberHistory(CommandInfo *info)
{
	StringList *node = info->history;
	int i = 0;

	while (node)
	{
		node->num = i++;
		node = node->next;
	}
	return (info->histcount = i);
}

