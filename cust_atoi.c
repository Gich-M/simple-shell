#include "shell.h"

/**
 * isInteractive - checks if the shell is in interactive mode
 * @info: pointer to struct CommandInfo.
 *          Contains arguments.
 *
 * Return: 1 if interactive mode, 0 otherwise
 */
int isInteractive(CommandInfo *info)
{
	return ((isatty(STDIN_FILENO) && info->input_fd <= 2));
}

/**
 * isDelim - checks if the character is a delimiter
 * @c: the character to check
 * @delim: the delimiter string
 *
 * Return: 1 if true, 0 if false
 */
int isDelim(char c, char *delim)
{
	while (*delim)
		if (*delim++ == c)
			return (1);
	return (0);
}

/**
 * isAlpha - checks for an alphabetic character
 *
 * @c: the character to check
 *
 * Return: 1 if 'c' is alphabetic, 0 otherwise
 */
int isAlpha(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (1);
	else
		return (0);
}

/**
 * _atoi - converts a string to an integer
 *
 * @s: the string to be converted
 *
 * Return: 0 if no numbers in the string, converted number otherwise
 */

int _atoi(char *s)
{
	int i, sign = 1, status = 0, output;
	unsigned int result = 0;

	for (i = 0; s[i] != '\0' && status != 2; i++)
	{
		if (s[i] == '-')
		{
			sign *= -1;
		}
		else if (s[i] >= '0' && s[i] <= '9')
		{
			status = 1;
			result *= 10;
			result += (s[i] - '0');
		}
		else if (status == 1)
			status = 2;
	}
	if (sign == -1)
		output = -result;
	else
		output = result;
	return (output);
}
