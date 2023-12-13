/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npirard <npirard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 15:46:52 by npirard           #+#    #+#             */
/*   Updated: 2023/12/13 17:07:53 by npirard          ###   ########.fr       */
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
		"number of cmds");
	while (pipex->commands && pipex->commands[i])
	{
		ft_printf("  %i\n", i);
		ft_print_strs(pipex->commands[i]);
		i++;
	}
}

bool	is_limiter(char *line, char *limiter)
{
	int	len_line;
	int	len_limiter;

	len_line = ft_strlen(line);
	len_limiter = ft_strlen(limiter);
	if (len_line && line[len_line - 1] == '\n')
	{
		if (len_line == len_limiter + 1
			&& !ft_strncmp(line, limiter, len_limiter))
			return (true);
	}
	else if (!ft_strcmp(line, limiter))
		return (true);
	return (false);
}

void	here_doc(int fd, char *limiter)
{
	char	*line;

	line = ft_gnl(0);
	while (line && !is_limiter(line, limiter))
	{
		write(fd, line, ft_strlen(line));
		free(line);
		line = ft_gnl(0);
	}
	dprintf(2, "STOP\n");
	if (line)
		free(line);
}

int	fd_to_fd(int fd_in, int fd_out)
{
	char	buffer[BUFFER_SIZE];
	size_t	byte_r;

	byte_r = read(fd_in, buffer, BUFFER_SIZE);
	while (byte_r > 0)
	{
		if (write(fd_out, buffer, byte_r))
		{
			byte_r = -1;
			break ;
		}
		byte_r = read(fd_in, buffer, BUFFER_SIZE);
	}
	if (byte_r < 0)
	{
		error("Error while writing to stdout : ", errno);
		return (errno);
	}
	return (-1);
}

/// @brief Handle redirection to stdin.
/// If it is the first command and a file_in is provided,
/// stdin is redirected to file.
/// If it is not the first command, stdin is redirected to the read
/// part of the previous fd.
/// @param pipex
/// @param old_fd Previous pipe fd. ```NULL``` for the first command.
/// @return ```0``` if no error occurred.
/// ```errno``` if an error occured opening a file.
int	handle_file_in(t_pipex *pipex, int *old_fd)
{
	int	fd_file_in;

	if (!old_fd && pipex->file_in)
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
 	/* else if (!old_fd && pipex->limiter)
	{
		//here_doc(0, pipex->limiter);
		int fd_temp = open("/home/npirard/Documents/pipex/.tmp", O_TRUNC | O_RDWR | O_CREAT, 777);
		here_doc(fd_temp, pipex->limiter);
		fd_to_fd(fd_temp, 0);
		close(fd_temp);

		//close(fd[0]);
	} */
	else if (old_fd)
	{
		dup2(old_fd[0], 0);
		close(old_fd[0]);
	}
	return (0);
}

/// @brief Handle redirection for stdout.
/// If it is the last command and a file_out is provided. stdout is redirected
/// to file.
/// Else stdout is redirected to the end of the pipe.
/// @param pipex
/// @param i_cmd
/// @param fd
/// @return ```0``` if no error occurred.
/// ```errno``` if an error occured opening a file.
int	handle_file_out(t_pipex *pipex, int i_cmd, int *fd)
{
	int	fd_out;

	if (i_cmd == (ft_strsslen(pipex->commands) - 1) && pipex->file_out)
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
	else if (i_cmd < (ft_strsslen(pipex->commands) - 1))
	{
		dup2(fd[1], 1);
		close(fd[1]);
	}
	return (0);
}

/// @brief Execute cmd in current process. If cmd can not be executed
/// an error message is print. Process exit in all cases.
/// @param cmd
/// @param env
/// @param fd
void	exec_cmd(char **cmd, char **env, int *fd)
{

	close(fd[0]);
	close(fd[1]);
	if (execve(cmd[0], cmd, env))
		error(cmd[0], errno);
	exit(errno);
}

/// @brief If cmd path is NULL (meaning cmd was given empty), write stdin
/// to stdout.
/// @param cmd
/// @return ```0``` if cmd path is not empty. ```-1``` if stdin was successfully
/// writen to stdout or if it was an empty string (meaning given path was wrong)
/// ```errno``` (> 1) if an error occured during reading
/// or writing.
int	check_empty_cmd(char **cmd)
{
	char	buffer[BUFFER_SIZE];
	size_t	byte_r;

	if (!cmd[0])
	{
		byte_r = read(0, buffer, BUFFER_SIZE);
		while (byte_r > 0)
		{
			if (write(1, buffer, byte_r))
			{
				byte_r = -1;
				break ;
			}
			byte_r = read(0, buffer, BUFFER_SIZE);
		}
		if (byte_r < 0)
		{
			error("Error while writing to stdout : ", errno);
			return (errno);
		}
		return (-1);
	}
	else if (cmd[0] && cmd[0][0] == '\0')
		return (-1);
	return (0);
}

void	exec_cmds(t_pipex *pipex, char **env, int *old_fd, int i_cmd)
{
	int		id;
	int		fd[2];

	pipe(fd);
	id = 0;
	if (!handle_file_out(pipex, i_cmd, fd)
		&& !handle_file_in(pipex, old_fd)
		&& !check_empty_cmd(pipex->commands[i_cmd]))
	{
		id = fork();
		if (id == 0)
		{
			if (pipex->limiter)
			{
				//here_doc(0, pipex->limiter);
				int fd_temp = open("/home/npirard/Documents/pipex/.tmp", O_TRUNC | O_RDWR | O_CREAT, 777);
				here_doc(fd_temp, pipex->limiter);
				fd_to_fd(fd_temp, fd[1]);
				close(fd_temp);
				//close(fd[0]);
			}
			exec_cmd(pipex->commands[i_cmd], env, fd); }
	}
	if (i_cmd < (ft_strsslen(pipex->commands) - 1))
		exec_cmds(pipex, env, fd, i_cmd + 1);
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
	exec_cmds(pipex, env, NULL, 0);
	pipex_clear(pipex, 0);
	return (0);
}
