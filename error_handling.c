#include "shell.h"

/**
 * ePuts - Prints an error message to stderr.
 *
 * @message: The error message to be printed.
 *
 * Return: Nothing.
 */
void ePuts(char *message)
{
	int i = 0;

	if (!message)
		return;
	while (message[i] != '\0')
	{
		ePutchar(message[i]);
		i++;
	}
}

/**
 * ePutchar - Writes a character to stderr.
 *
 * @c: The character to be printed.
 *
 * Return: On success 1.
 *         On error, -1 is returned, and errno is set appropriately.
 */
int ePutchar(char c)
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
 * putFd - Writes a character to the given file descriptor.
 *
 * @c: The character to be printed.
 *
 * @fd: The file descriptor to write to.
 *
 * Return: On success 1.
 *         On error, -1 is returned, and errno is set appropriately.
 */
int putFd(char c, int fd)
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
 * putsFd - Prints a string to the given file descriptor.
 *
 * @str: The string to be printed.
 *
 * @fd: The file descriptor to write to.
 *
 * Return: The number of characters printed.
 */
int putsFd(char *str, int fd)
{
	int count = 0;

	if (!str)
		return (0);
	while (*str)
	{
		count += putFd(*str++, fd);
	}
	return (count);
}

