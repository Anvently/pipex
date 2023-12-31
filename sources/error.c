/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npirard <npirard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 15:46:25 by npirard           #+#    #+#             */
/*   Updated: 2023/12/14 15:26:02 by npirard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pipex.h>
#include <errno.h>
#include <libft.h>
#include <string.h>

/// @brief
/// @param error
/// @return
/// @note ```1``` unsufficient number of args.
/// @note ```2``` append flag given without file.
/// @note ```3``` no PATH variable in env.
/// @note ```4``` access error.
int	error_input(int error)
{
	if (error == 1)
		ft_putendl_fd("Unsufficient number of arguments were given.", 2);
	else if (error == 2)
		ft_putendl_fd("Append flag (-a) was given without file.", 2);
	else if (error == 3)
		ft_putendl_fd("No PATH variable was found in env.", 2);
	return (error);
}

int	error_path(char *msg, char *path)
{
	ft_putstr_fd(msg, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(path, 2);
	return (1);
}

int	error_alloc(void)
{
	ft_putendl_fd("Allocation error.", 2);
	return (1);
}

int	error(char *error_msg, int error)
{
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(error_msg, 2);
	return (error);
}

/// @brief Close given fd and return error.
/// @param fd
/// @param error
/// @return error
int	error_close(int fd, int error)
{
	close(fd);
	return (error);
}
