/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npirard <npirard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 15:46:52 by npirard           #+#    #+#             */
/*   Updated: 2023/12/13 13:00:14 by npirard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pipex.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdio.h>

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

int	handle_file_in(t_pipex *pipex, int i_command, int *old_fd)
{
	int	fd_file_in;

	if (i_command == 0 && pipex->file_in)
	{
		fd_file_in = open(pipex->file_in, O_RDONLY);
		if (fd_file_in < 0)
		{
			error(pipex->file_in, errno);
			return (errno);
		}
		dup2(fd_file_in, 0);
		close(fd_file_in);
	}
	else if (old_fd)
	{
		dup2(old_fd[0], 0);
		close(old_fd[0]);
	}
	return (0);
}

int	handle_file_out(t_pipex *pipex, int i_command, int *fd)
{
	int	fd_out;

	if (i_command == (ft_strsslen(pipex->commands) - 1) && pipex->file_out)
	{
		if (pipex->flag_a)
			fd_out = open(pipex->file_out, O_APPEND | O_RDWR | O_CREAT, 777);
		else
			fd_out = open(pipex->file_out, O_RDWR | O_TRUNC | O_CREAT, 777);
		if (fd_out < 0)
		{
			error(pipex->file_out, errno);
			return (errno);
		}
		dup2(fd_out, 1);
		close(fd_out);
	}
	else if (i_command < (ft_strsslen(pipex->commands) - 1))
	{
		dup2(fd[1], 1);
		close(fd[1]);
	}
	return (0);
}

void	exec_cmd(char **cmd, char **env, bool err, int *fd)
{
	int	id;

	id = fork();

	if (id == 0)
	{
		close(fd[0]);
		close(fd[1]);
		if (!err && execve(cmd[0], cmd, env))
			error(cmd[0], errno);
		exit(err);
	}
}

void	exec_commands(t_pipex *pipex, char **env, int *old_fd, int i_command)
{
	int		id;
	int		fd[2];
	bool	err;

	id = fork();
	pipe(fd);
	err = false;

	if (id == 0)
	{
		err = handle_file_out(pipex, i_command, fd);
		if (!err)
			err = handle_file_in(pipex, i_command, old_fd);
		exec_cmd(pipex->commands[i_command], env, err, fd);
		if (i_command < (ft_strsslen(pipex->commands) - 1))
			exec_commands(pipex, env, fd, i_command + 1);
		close(fd[0]);
		close(fd[1]);
		wait(NULL);
		exit(0);
	}
	close(fd[0]);
	close(fd[1]);
	waitpid(id, NULL, 0);
}

int	main(int argc, char **argv, char **env)
{
	t_pipex	*pipex;

	printf("%d\n", (int) getpid());
	pipex = parse_input(argc, argv, env);
	print_pipex(pipex);
	exec_commands(pipex, env, NULL, 0);
	pipex_clear(pipex, 0);
	return (0);
}
