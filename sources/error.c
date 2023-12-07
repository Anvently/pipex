/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npirard <npirard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 15:46:25 by npirard           #+#    #+#             */
/*   Updated: 2023/12/07 17:37:11 by npirard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pipex.h>
#include <errno.h>

int	error_input(int error)
{
	if (error == 1)
		ft_putendl_fd("Unsufficient number of arguments were given.", 2);
	return (error);
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
