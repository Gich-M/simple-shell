#include "shell.h"

/**
 * isCmd - Checks if a file is an executable command.
 * @info: pointer to struct CommandInfo.
 *          Contains arguments.
 * @path: Path to the file.
 *
 * Return: 1 if true, 0 otherwise.
 */
int isCmd(CommandInfo *info, char *path)
{
	struct stat st;

	(void)info;

	if (!path || stat(path, &st))
	{
		return (0);
	}
	if (st.st_mode & S_IFREG)
	{
		return (1);
	}
	return (0);
}

/**
 * dupChars - Duplicates characters from a given string.
 * @pathstr: The PATH string.
 * @start: Starting index.
 * @stop: Stopping index.
 *
 * Return: Pointer to the new buffer.
 */
char *dupChars(char *pathstr, int start, int stop)
{
	static char buf[1024];
	int i = start, j = 0;

	for (; i < stop; i++)
		if (pathstr[i] != ':')
			buf[j++] = pathstr[i];
	buf[j] = '\0';
	return (buf);
}

/**
 * findPath - Finds the full path of a command in the PATH string.
 * @info: The info struct.
 * @pathstr: The PATH string.
 * @cmd: The command to find.
 *
 * Return: Full path of cmd if found, otherwise NULL.
 */
char *findPath(CommandInfo *info, char *pathstr, char *cmd)
{
	int i = 0, curr_pos = 0;
	char *path;

	if (!pathstr)
		return (NULL);
	if ((_strlen(cmd) > 2) && startsWith(cmd, "./"))
	{
		if (isCmd(info, cmd))
			return (cmd);
	}
	while (1)
	{
		if (!pathstr[i] || pathstr[i] == ':')
		{
			path = dupChars(pathstr, curr_pos, i);

			if (!*path)
				_strcat(path, cmd);
			else
			{
				_strcat(path, "/");
				_strcat(path, cmd);
			}
			if (isCmd(info, path))
				return (path);
			if (!pathstr[i])
				break;
			curr_pos = i;
		}
		i++;
	}
	return (NULL);
}

