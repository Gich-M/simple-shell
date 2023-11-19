#include "shell.h"

/**
 * strError - converts string to integer.
 *
 * @s: string to be converted
 *
 * Return: the converted integer if successful, or -1 on error.
 */
int strError(char *s)
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
 * printError - Prints an error message.
 *
 * @info: pointer to struct CommandInfo.
 *          Contains arguments.
 *
 * @estr: String containing the specified error type.
 *
 * Return: Always 0 if no numbers in string, converted number otherwise,
 *         -1 on error.
 */
void printError(CommandInfo *info, char *estr)
{
	ePuts(info->fname);
	ePuts(": ");
	printDec(info->line_count, STDERR_FILENO);
	ePuts(": ");
	ePuts(info->argv[0]);
	ePuts(": ");
	ePuts(estr);
}

/**
 * printDec - Prints a decimal (integer) number (base 10).
 *
 * @input: The input number to be printed.
 *
 * @fd: The file descriptor to write to.
 *
 * Return: The number of characters printed.
 */

int printDec(int input, int fd)
{
	int (*__putchar)(char) = _putchar;
	int i, count = 0;
	unsigned int absVal, current;

	if (fd == STDERR_FILENO)
		__putchar = ePutchar;
	if (input < 0)
	{
		absVal = -input;
		__putchar('-');
		count++;
	}
	else
		absVal = input;
	current = absVal;
	for (i = 1000000000; i > 1; i /= 10)
	{
		if (absVal / i)
		{
			__putchar('0' + current / i);
			count++;
		}
		current %= i;
	}
	__putchar('0' + current);
	count++;

	return (count);
}

/**
 * convertNum - converts a number to a string
 * @num: the number to be converted
 * @base: the base for conversion
 * @flags: argument flags
 *
 * Return: the converted string
 */
char *convertNum(long int num, int base, int flags)
{
	static char *array;
	static char buffer[50];
	char sign = 0;
	char *ptr;
	unsigned long n = num;

	if (!(flags & CONVERT_UNSIGNED) && num < 0)
	{
		n = -num;
		sign = '-';

	}
	array = flags & CONVERT_LOWERCASE ? "0123456789abcdef" : "0123456789ABCDEF";
	ptr = &buffer[49];
	*ptr = '\0';

	do	{
		*--ptr = array[n % base];
		n /= base;
	} while (n != 0);

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

