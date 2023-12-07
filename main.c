#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int	main(void)
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
}