/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_util.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npirard <npirard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 11:37:13 by npirard           #+#    #+#             */
/*   Updated: 2023/12/14 17:42:20 by npirard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pipex.h>
#include <errno.h>
#include <unistd.h>
#include <libft.h>

/// @brief Write content of fd_in to fd_out.
/// @param fd_in
/// @param fd_out
/// @return ```0``` on success. ```errno``` on error.
int	fd_to_fd(int fd_in, int fd_out)
{
	char	buffer[BUFFER_SIZE];
	size_t	byte_r;

	byte_r = read(fd_in, buffer, BUFFER_SIZE);
	while (byte_r > 0)
	{
		if (write(fd_out, buffer, byte_r) < 0)
		{
			byte_r = -1;
			break ;
		}
		byte_r = read(fd_in, buffer, BUFFER_SIZE);
	}
	if (byte_r < 0)
		return (errno);
	return (0);
}
