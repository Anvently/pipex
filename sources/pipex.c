/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npirard <npirard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 15:46:52 by npirard           #+#    #+#             */
/*   Updated: 2023/12/14 18:13:22 by npirard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pipex.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <errno.h>
#include <libft.h>

/// @brief Handle redirection to stdin.
/// If it is the first command and a file_in is provided,
/// stdin is redirected to file.
/// If it is not the first command, stdin is redirected to the read
/// part of the previous fd.
/// @param pipex
/// @param old_fd Previous pipe fd. ```NULL``` for the first command.
/// @return ```0``` if no error occurred.
/// ```errno``` if an error occured opening a file.
static int	handle_file_in(t_pipex *pipex, int *old_fd)
{
	int	fd_file_in;

	if (!old_fd && pipex->file_in)
	{
		fd_file_in = open(pipex->file_in, O_RDONLY);
		if (fd_file_in < 0)
			return (error(pipex->file_in, errno));
		if (dup2(fd_file_in, 0) < 0)
			return (error("", error_close(fd_file_in, errno)));
		close(fd_file_in);
	}
	else if (!old_fd && pipex->limiter && here_doc(pipex->limiter))
		return (error("here document", errno));
	else if (old_fd)
	{
		if (dup2(old_fd[0], 0) < 0)
			return (error("", error_close(old_fd[0], errno)));
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
static int	handle_file_out(t_pipex *pipex, int i_cmd, int *fd)
{
	int	fd_out;

	if (i_cmd == (ft_strsslen(pipex->commands) - 1) && pipex->file_out)
	{
		if (pipex->flag_a)
			fd_out = open(pipex->file_out, O_APPEND | O_RDWR | O_CREAT, 0644);
		else
			fd_out = open(pipex->file_out, O_RDWR | O_TRUNC | O_CREAT, 0644);
		if (fd_out < 0)
			return (error(pipex->file_out, errno));
		if (dup2(fd_out, 1) < 0)
			return (error("", error_close(fd_out, errno)));
		close(fd_out);
	}
	else if (i_cmd < (ft_strsslen(pipex->commands) - 1))
	{
		if (dup2(fd[1], 1) < 0)
			return (error("", error_close(fd[1], errno)));
		close(fd[1]);
	}
	return (0);
}

/// @brief Execute cmd in current process. If cmd can not be executed
/// an error message is print. Process exit in all cases.
/// @param cmd
/// @param env
/// @param fd
static void	exec_cmd(char **cmd, char **env, int *fd)
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
static int	check_empty_cmd(char **cmd)
{
	char	buffer[100];

	if (!cmd[0])
	{
		if (fd_to_fd(0, 1))
			return (error("error while writing to pipe", errno));
		return (-1);
	}
	else if (cmd[0] && cmd[0][0] == '\0')
	{
		while (read(0, buffer, 100) > 0)
			continue ;
		return (-1);
	}
	return (0);
}

int	exec_cmds(t_pipex *pipex, char **env, int *old_fd, int i_cmd)
{
	int		id;
	int		fd[2];
	int		status;

	status = 0;
	pipe(fd);
	id = 0;
	if (!handle_file_out(pipex, i_cmd, fd)
		&& !handle_file_in(pipex, old_fd)
		&& !check_empty_cmd(pipex->commands[i_cmd]))
	{
		id = fork();
		if (id == 0)
			exec_cmd(pipex->commands[i_cmd], env, fd);
	}
	if (i_cmd < (ft_strsslen(pipex->commands) - 1))
		status = exec_cmds(pipex, env, fd, i_cmd + 1);
	close(fd[0]);
	close(fd[1]);
	close(0);
	if (i_cmd < (ft_strsslen(pipex->commands) - 1))
		waitpid(id, NULL, 0);
	else
		waitpid(id, &status, 0);
	return (status);
}
