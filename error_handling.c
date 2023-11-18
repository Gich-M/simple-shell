#include "shell.h"

/**
 * printError - Prints an error message to stderr.
 *
 * @message: The error message to be printed.
 *
 * Return: Nothing.
 */
void printError(char *message)
{
	int i = 0;

	if (!message)
		return;
	while (message[i] != '\0')
	{
		writeErrorChar(message[i]);
		i++;
	}
}

/**
 * writeErrorChar - Writes a character to stderr.
 *
 * @c: The character to be printed.
 *
 * Return: On success 1.
 *         On error, -1 is returned, and errno is set appropriately.
 */
int writeErrorChar(char c)
{
	static int i;
	static char buf[WRITE_BUFFER];

	if (c == FLUSH_BUFFER || i >= WRITE_BUFFER)
	{
		write(STDERR_FILENO, buf, i);
		i = 0;
	}
	if (c != FLUSH_BUFFER)
		buf[i++] = c;
	return (1);
}

/**
 * writeCharToFD - Writes a character to the given file descriptor.
 *
 * @c: The character to be printed.
 *
 * @fd: The file descriptor to write to.
 *
 * Return: On success 1.
 *         On error, -1 is returned, and errno is set appropriately.
 */
int writeCharToFD(char c, int fd)
{
	static int i;
	static char buf[WRITE_BUFFER];

	if (c == FLUSH_BUFFER || i >= WRITE_BUFFER)
	{
		write(fd, buf, i);
		i = 0;
	}
	if (c != FLUSH_BUFFER)
		buf[i++] = c;
	return (1);
}

/**
 * printStringToFD - Prints a string to the given file descriptor.
 *
 * @str: The string to be printed.
 *
 * @fd: The file descriptor to write to.
 *
 * Return: The number of characters printed.
 */
int printStringToFD(char *str, int fd)
{
	int count = 0;

	if (!str)
		return (0);
	while (*str)
	{
		count += writeCharToFD(*str++, fd);
	}
	return (count);
}

