/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npirard <npirard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 15:46:52 by npirard           #+#    #+#             */
/*   Updated: 2023/12/12 19:04:56 by npirard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pipex.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <errno.h>

static void	print_pipex(t_pipex *pipex)
{
	int	i;

	i = 0;
	ft_printf(
		"-- %7$-*1$s = %2$s\n"
		"-- %8$-*1$s = %3$s\n"
		"-- %9$-*1$s = %4$i\n"
		"-- %10$-*1$s = %5$s\n"
		"-- %11$-*1$s = %6$d\n",
		20,
		pipex->file_in,
		pipex->file_out,
		pipex->flag_a,
		pipex->limiter,
		ft_strsslen(pipex->commands),
		"pipex->file_in", "pipex->file_out", "flag_a", "limiter",
		"number of commands");
	while (pipex->commands && pipex->commands[i])
	{
		ft_printf("  %i\n", i);
		ft_print_strs(pipex->commands[i]);
		i++;
	}
}

/* static void	pipex_exec(t_pipex *pipex)
{
	// If input, open file
		// If error
			// Go to next command
		// Dup file to stdin
		// Fork process
			// If fork fail
				// Close file write end
				// Return ?
		// Exec command
		// Go to next command
			//
	// If stdin
		// Exec
} */

int	check_file_in(t_pipex *pipex, int i_command)
{
	int	fd_file_in;

	if (i_command == 0 && pipex->file_in)
	{
		fd_file_in = open(pipex->file_in, O_RDONLY);
		if (pipex->file_in && fd_file_in < 0)
		{
			error(pipex->file_in, errno);
			return (1);
		}
		else
			dup2(fd_file_in, 0);
		close(fd_file_in);
	}
	return (0);
}

int	handle_file_out(t_pipex *pipex, int i_command, int *fd)
{
	int	fd_file_in;

	if (i_command == (ft_strsslen(pipex->commands) - 1) && pipex->file_out)
	{
		if (pipex->flag_a)
			fd_file_in = open(pipex->file_out,
					O_APPEND | O_RDWR | O_CREAT, 777);
		else
			fd_file_in = open(pipex->file_out, O_RDWR | O_TRUNC | O_CREAT, 777);
		if (fd_file_in < 0)
		{
			error(pipex->file_out, errno);
			return (1);
		}
		dup2(fd_file_in, 1);
		close(fd_file_in);
	}
	else if (i_command < (ft_strsslen(pipex->commands) - 1))
		dup2(fd[1], 1);
	return (0);
}

/* void	exec_command(char **command, char **env)
{
	int fd_file_in[2];
	int	id1;
	int	id2;

	pipe(fd_file_in);
	id1 = fork();
	id2 = 0;

	if (id1 == 0)
	{
		close(fd_file_in[0]);
		dup2(fd_file_in[1], 1);
		execve(pipex->commands[0][0], pipex->commands[0], env);
	}
	else
	{
		id2 = fork();
		if (id2 == 0)
		{
			close(fd_file_in[1]);
			dup2(fd_file_in[0], 0);
			execve(pipex->commands[1][0], pipex->commands[1], env);
		}
	}
	close(fd_file_in[0]);
	close(fd_file_in[1]);
	waitpid(id1, NULL, 0);
	waitpid(id2, NULL, 0);
} */

void	exec_commands(t_pipex *pipex, char **env, int *old_fd, int i_command)
{
	int		id;
	int		fd[2];
	bool	err;

	id = fork();
	int id2;
	pipe(fd);
	err = false;

	if (id == 0)
	{
		ft_printf("Passage i=%d|old_fd[0]=%i|old_fd[1]=%i|fd[0]=%i|fd[1]=%i\n", i_command,
		old_fd ? old_fd[0] : -1, old_fd ? old_fd[1] : -1, fd[0], fd[1]);
		if (handle_file_out(pipex, i_command, fd)) {
			err = true; printf("Error on file_out !\n");}
		if (check_file_in(pipex, i_command)) {
			err = true; printf("Error on file_in !\n");}
		else if (old_fd)
			dup2(old_fd[0], 0);
		if (!err && execve(pipex->commands[i_command][0],
			pipex->commands[i_command], env))
			exit(error(pipex->commands[i_command][0], errno));
		if (i_command < (ft_strsslen(pipex->commands) - 1)){
			id2 = fork();
			if (id2 == 0)
				exec_commands(pipex, env, fd, i_command + 1);
			waitpid(id2, NULL, 0);}
	}
	waitpid(id, NULL, 0);
	close(fd[0]);
	close(fd[1]);
}

int	main(int argc, char **argv, char **env)
{
	t_pipex	*pipex;

	pipex = parse_input(argc, argv, env);
	print_pipex(pipex);
	exec_commands(pipex, env, NULL, 0);
	pipex_clear(pipex, 0);
	return (0);
}
