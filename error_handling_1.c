#include "shell.h"

/**
 * printErrorString - converts string to integer.
 *
 * @s: string to be converted
 *
 * Return: the converted integer if successful, or -1 on error.
 */
int printErrorString(char *s)
{
	int i = 0;
	unsigned long int result = 0;

	if (*s == '+')
		s++;
	for (i = 0; s[i] != '\0'; i++)
	{
		if (s[i] >= '0' && s[i] <= '9')
		{
			result = result * 10 + (s[i] - '0');
			if (result > INT_MAX)
				return (-1);
		}
		else
		{
			return (-1);
		}
	}
	return (result);
}

/**
 * printErrorInfo - Prints an error message.
 *
 * @info: pointer to struct CommandInfo.
 *          Contains arguments.
 *
 * @estr: String containing the specified error type.
 *
 * Return: Always 0 if no numbers in string, converted number otherwise,
 *         -1 on error.
 */
void printErrorInfo(CommandInfo *info, char *estr)
{
	_puts(info->fname);
	_puts(": ");
	printDecimal(info->line_count, STDERR_FILENO);
	_puts(": ");
	_puts(info->argv[0]);
	_puts(": ");
	_puts(estr);
}

/**
 * printDecimal - Prints a decimal (integer) number (base 10).
 *
 * @input: The input number to be printed.
 *
 * @fd: The file descriptor to write to.
 *
 * Return: The number of characters printed.
 */

int printDecimal(int input, int fd)
{
	int (*outputChar)(char) = (fd == STDERR_FILENO) ? _putchar : _putchar;
	int i, count = 0;
	unsigned int absoluteValue, current;

	if (input < 0)
	{
		absoluteValue = -input;
		outputChar('-');
		count++;
	}
	else
	{
		absoluteValue = input;
	}
	current = absoluteValue;

	for (i = 1000000000; i > 1; i /= 10)
	{
		if (absoluteValue / i)
		{
			outputChar('0' + current / i);
			count++;
		}
		current %= i;
	}
	outputChar('0' + current);
	count++;
	return (count);
}

/**
 * convertNumber - converts a number to a string
 * @num: the number to be converted
 * @base: the base for conversion
 * @flags: argument flags
 *
 * Return: the converted string
 */
char *convertNumber(long int num, int base, int flags)
{
	static char buffer[50];
	char *ptr = &buffer[49];
	char *array =
		(flags & CONVERT_LOWERCASE) ? "0123456789abcdef" : "0123456789ABCDEF";
	char sign = 0;

	if (!(flags & CONVERT_UNSIGNED) && num < 0)
	{
		num = -num;
		sign = '-';
	}
	*ptr = '\0';

	do {
		*--ptr = array[num % base];
		num /= base;
	} while (num != 0);

	if (sign)
		*--ptr = sign;
	return (ptr);
}

/**
 * removeComments - Replaces the first instance of '#' with '\0'.
 *
 * @str: buffer address of string to modify.
 *
 * Return: Always 0.
 */
void removeComments(char *str)
{
	int i;

	for (i = 0; str[i] != '\0'; i++)
	{
		if (str[i] == '#' && (!i || str[i - 1] == ' '))
		{
			str[i] = '\0';
			break;
		}
	}
}

