#include "shell.h"

/**
 * **strtow - splits a string into words
 * @str: string to split
 * @delim: delimeter string
 *
 * Return: pointer to an array of strings (Success)
 * or NULL (Error)
 */

char **strtow(char *str, char *delim)
{
	int i, j, k, m, numWords = 0, inWord = 0;
	char **wordsArray;

	if (!str || !str[0])
		return (NULL);
	delim = delim ? delim : " ";
	for (i = 0; str[i]; i++)
	{
		if (str[i] == ' ' || strchr(delim, str[i]))
			inWord = 0;
		else if (!inWord)
			inWord = 1, numWords++;
	}
	if (!numWords)
		return (NULL);
	wordsArray = malloc((1 + numWords) * sizeof(char *));
	if (!wordsArray)
		return (NULL);
	for (i = 0, j = 0; j < numWords; j++)
	{
		while (str[i] == ' ' || strchr(delim, str[i]))
			i++;
		k = 0;
		while (str[i + k] != ' ' && str[i + k] && !strchr(delim, str[i + k]))
			k++;
		wordsArray[j] = malloc((k + 1) * sizeof(char));
		if (!wordsArray[j])
		{
			for (k = 0; k < j; k++)
				free(wordsArray[k]);
			free(wordsArray);
			return (NULL);
		}
		for (m = 0; m < k; m++)
			wordsArray[j][m] = str[i++];
		wordsArray[j][m] = '\0';
	}
	wordsArray[j] = NULL;
	return (wordsArray);
}

/**
 * **strtow2 - splits a string into words
 * @str: the input string
 * @delim: the delimeter
 * Return: a pointer to an array of strings, or NULL on failure
 */

char **strtow2(char *str, char delim)
{
	int i, j, k, m, numWords = 0;
	char **wordsArray;

	if (!str || !str[0])
		return (NULL);
	for (i = 0; str[i]; i++)
	{
		if ((str[i] != delim && str[i + 1] == delim) ||
				(str[i] != delim && !str[i + 1]) || str[i + 1] == delim)
			numWords++;
	}
	if (!numWords)
		return (NULL);
	wordsArray = malloc((1 + numWords) * sizeof(char *));
	if (!wordsArray)
		return (NULL);
	for (i = 0, j = 0; j < numWords; j++)
	{
		while (str[i] == delim && str[i] != delim)
			i++;
		k = 0;
		while (str[i + k] != delim && str[i + k] && str[i + k] != delim)
			k++;
		wordsArray[j] = malloc((k + 1) * sizeof(char));
		if (!wordsArray[j])
		{
			for (k = 0; k < j; k++)
				free(wordsArray[k]);
			free(wordsArray);
			return (NULL);
		}
		for (m = 0; m < k; m++)
			wordsArray[j][m] = str[i++];
		wordsArray[j][m] = '\0';
	}
	wordsArray[j] = (NULL);
	return (wordsArray);
}

