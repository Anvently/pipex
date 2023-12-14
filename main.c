#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

/* int	main(void)
{
	char buffer[100000] = {0};
	char buffer2[100000] = {1};
	int fd[2];

	if (pipe(fd))
		return (1);
	int id = fork();
	if (!id)
	{
		close(fd[1]);
		if (write(fd[1], buffer, 100000) < 0)
			printf("error\n");
		close(fd[1]);
	}
	else
	{
		close(fd[1]);
		if (read(fd[0], buffer2, 100000) <= 0)
			printf("error\n");
		printf("rwer\n");
		close(fd[0]);
	}
	waitpid(id, NULL, 0);
	printf("test\n");
	wait(NULL);
	return (0);
} */

/* char	**test(char **strs)
{
	int i;

	i = 0;
	while (*strs)
		if(*strs++)
			i++;
	return (strs);
} */

void print_bits(unsigned char octet)
{
	int	i = 8;
	unsigned char 	bit;

	while (i--)
	{
		bit = (octet >> i & 1) + '0';
		write(1, &bit, 1);
	}
}

int	main(void)
{
	int byte;
	int	bit;

	byte = 256;
	bit = '0' + (3 & 1);
	print_bits((unsigned char) byte);
	//write(1, &bit, 1);
	return (0);
}
