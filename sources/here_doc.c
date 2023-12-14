/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npirard <npirard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 09:48:27 by npirard           #+#    #+#             */
/*   Updated: 2023/12/14 11:52:44 by npirard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pipex.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <libft.h>

/// @brief Check if given line is limiter
/// @param line Can end with or without \\n. \\n is ignored from the
/// comparison.
/// @param limiter
/// @return ```true``` if line is limiter else ```false```.
static bool	is_limiter(char *line, char *limiter)
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

/// @brief Read lines from stdin, writing them in given fd until limiter
/// is encountered
/// @param fd
/// @param limiter
/// @return ```0``` on success. ```errno``` on error.
static int	take_input(int fd, char *limiter)
{
	char	*line;

	line = ft_gnl(0);
	while (line && !is_limiter(line, limiter))
	{
		if (write(fd, line, ft_strlen(line)) < 0)
		{
			free(line);
			return (errno);
		}
		free(line);
		line = ft_gnl(0);
	}
	if (line)
		free(line);
	return (0);
}

/// @brief Write stdin into tmp file
/// @param limiter
/// @return ```0``` on success. ```errno``` on error.
static int	write_tmp_file(char *limiter)
{
	int		fd_temp;

	fd_temp = open(HERE_DOC_TMP_PATH, O_TRUNC | O_RDWR | O_CREAT, 0644);
	if (fd_temp < 0)
		return (errno);
	if (take_input(fd_temp, limiter))
	{
		unlink(HERE_DOC_TMP_PATH);
		return (error_close(fd_temp, errno));
	}
	close(fd_temp);
	return (0);
}

/// @brief Write stdin into tmp file until limiter is encountered,
/// then dup tmp file to stdin.
/// @param limiter
/// @return ```0``` on success. ```errno``` on error.
int	here_doc(char *limiter)
{
	int	fd_temp;

	if (write_tmp_file(limiter))
		return (errno);
	fd_temp = open(HERE_DOC_TMP_PATH, O_RDONLY);
	if (fd_temp < 0)
	{
		unlink(HERE_DOC_TMP_PATH);
		return (errno);
	}
	if (dup2(fd_temp, 0) < 0)
	{
		unlink(HERE_DOC_TMP_PATH);
		return (error_close(fd_temp, errno));
	}
	close(fd_temp);
	if (unlink(HERE_DOC_TMP_PATH))
		return (errno);
	return (0);
}
