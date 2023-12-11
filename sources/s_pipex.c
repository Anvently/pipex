/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   s_pipex.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npirard <npirard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 16:15:18 by npirard           #+#    #+#             */
/*   Updated: 2023/12/11 15:27:48 by npirard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pipex.h>

t_pipex	*pipex_init(void)
{
	t_pipex	*pipex;

	pipex = malloc(sizeof(t_pipex));
	if (!pipex)
		return (NULL);
	pipex->commands = NULL;
	pipex->env = NULL;
	pipex->file_in = NULL;
	pipex->file_out = NULL;
	pipex->nbr_commands = 0;
	pipex->flag_a = false;
	pipex->limiter = NULL;
	return (pipex);
}

int	pipex_clear(t_pipex *pipex, int error)
{
	if (!pipex)
		return (0);
	if (pipex->commands)
		ft_free_strss(pipex->commands);
	if (pipex->file_in)
		free(pipex->file_in);
	if (pipex->file_out)
		free(pipex->file_out);
	if (pipex->limiter)
		free(pipex->limiter);
	free(pipex);
	return (error);
}
