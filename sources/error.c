/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npirard <npirard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 15:46:25 by npirard           #+#    #+#             */
/*   Updated: 2023/12/08 14:42:46 by npirard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pipex.h>
#include <errno.h>

/// @brief
/// @param error
/// @return
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
	perror(error_msg);
	return (error);
}
