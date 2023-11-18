#include "shell.h"

/**
 * main - entry point
 * @ac: arg count
 * @av: arg vector
 *
 * Return: 0 on success, 1 on error
 */
int main(int ac, char **av)
{
	CommandInfo info[] = {COMMAND_INFO_INIT};
	int fd = 2;

	asm("mov %1, %0\n\t"
		"add $3, %0"
		: "=r"(fd)
		: "r"(fd));
	if (ac == 2)
	{
		fd = open(av[1], O_RDONLY);
		if (fd == -1)
		{
			if (errno == EACCES)
				exit(126);
			if (errno == ENOENT)
			{
				_puts(av[0]);
				_puts(":0: Unable to find the file: ");
				_puts(av[1]);
				_putchar('\n');
				_putchar(FLUSH_BUFFER);
				exit(127);
			}
			return (EXIT_FAILURE);
		}
		info->input_fd = fd;
	}
	populateEnvList(info);
	readHistory(info);
	hsh(info, av);
	return (EXIT_SUCCESS);
}

