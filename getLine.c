#include "shell.h"

/**
 * readBuffer - Reads a buffer from the given file descriptor.
 *
 * @info: pointer to struct CommandInfo.
 *          Contains arguments.
 *
 * @buffer: Buffer.
 *
 * @size: Size.
 *
 * Return: Number of bytes read.
 */
ssize_t readBuffer(CommandInfo *info, char *buffer, size_t *size)
{
	ssize_t bytesRead = 0;

	if (*size)
		return (0);
	bytesRead = read(info->input_fd, buffer, READ_BUFFER);

	if (bytesRead >= 0)
		*size = bytesRead;
	return (bytesRead);
}

/**
 * getLine - Reads the next line of input from STDIN.
 *
 * @info: pointer to struct CommandInfo.
 *          Contains arguments.
 *
 * @line: Address of pointer to buffer, preallocated or NULL.
 *
 * @length: Size of preallocated line buffer if not NULL.
 *
 * Return: Length of the input line.
 */
int getLine(CommandInfo *info, char **line, size_t *length)
{
	static char inputBuf[READ_BUFFER];
	static size_t bufIndex, bufLength;
	size_t lineLen;
	ssize_t bytesRead = 0;
	char *curLine = NULL, *newLine = NULL, *newlinePos;

	curLine = *line;

	if (curLine && length)
		lineLen = *length;
	if (bufIndex == bufLength)
		bufIndex = bufLength = 0;
	bytesRead = readBuffer(info, inputBuf, &bufLength);

	if (bytesRead == -1 || (bytesRead == 0 && bufLength == 0))
		return (-1);

	newlinePos = _strchr(inputBuf + bufIndex, '\n');
	lineLen = newlinePos ? 1 + (unsigned int)(newlinePos - inputBuf) : bufLength;
	newLine = _realloc(curLine, lineLen,
			(curLine && length) ? lineLen + *length : lineLen + 1);

	if (!newLine)
		return (curLine ? free(curLine), -1 : -1);
	if (curLine && length)
		_strncat(newLine, inputBuf + bufIndex, bytesRead);
	else
		_strncpy(newLine, inputBuf + bufIndex, bytesRead + 1);
	lineLen += bytesRead;
	bufIndex += bytesRead;
	curLine = newLine;

	if (length)
		*length = lineLen;
	*line = curLine;
	return (lineLen);
}

/**
 * bufferInput - buffers chained commands from the input stream
 *
 * @info: pointer to struct CommandInfo.
 *          Contains arguments.
 *
 * @buffer: Address of the buffer.
 *
 * @length: Address of the length variable.
 *
 * Return: Number of bytes read.
 */
ssize_t bufferInput(CommandInfo *info, char **buffer, size_t *length)
{
	ssize_t bytesRead = 0;
	size_t bufferLength = 0;

	if (!*length)
	{
		free(*buffer);
		*buffer = NULL;
		signal(SIGINT, sigintHandler);

#if USE_CUSTOM_READ
		bytesRead = getline(buffer, &bufferLength, stdin);
#else
		bytesRead = getline(buffer, &bufferLength, stdin);
#endif
		if (bytesRead > 0)
		{
			if ((*buffer)[bytesRead - 1] == '\n')
			{
				(*buffer)[bytesRead - 1] = '\0';
				bytesRead--;
			}
			info->linecount_flag = 1;
			removeComments(*buffer);
			buildHistoryList(info, *buffer, info->histcount++);

			if (_strchr(*buffer, ';'))
			{
				*length = bytesRead;
				info->cmd_buf = buffer;
			}
		}
	}
	return (bytesRead);
}

/**
 * getInput - Gets a line minus the newline.
 *
 * @info: pointer to struct CommandInfo.
 *          Contains arguments.
 *
 * Return: Bytes read.
 */
ssize_t getInput(CommandInfo *info)
{
	static char *cmdBuffer;
	static size_t bufIndex, currIndex, bufLength;
	ssize_t bytesRead = 0;
	char **currCmd = &(info->arg), *currPos;

	_putchar(FLUSH_BUFFER);
	bytesRead = bufferInput(info, &cmdBuffer, &bufLength);

	if (bytesRead == -1)
		return (-1);
	if (bufLength)
	{
		currIndex = bufIndex;
		currPos = cmdBuffer + bufIndex;

		checkChain(info, cmdBuffer, &currIndex, bufIndex, bufLength);
		while (currIndex < bufLength)
		{
			if (isChainDelimiter(info, cmdBuffer, &currIndex))
				break;
			currIndex++;
		}
		bufIndex = currIndex + 1;

		if (bufIndex >= bufLength)
		{
			bufIndex = bufLength = 0;
			info->cmd_buf_type = NORMAL_COMMAND;
		}
		*currCmd = currPos;
		return (_strlen(currPos));
	}
	*currCmd = cmdBuffer;
	return (bytesRead);
}

/**
 * sigintHandler -  Blocks Ctrl-C signal
 *
 * @sig_num: The signal number.
 *
 * Return: void
 */
void sigintHandler(__attribute__((unused)) int sig_num)
{
	_puts("\n");
	_puts("Custom_Shell$ ");
	_putchar(FLUSH_BUFFER);
}

