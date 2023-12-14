/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   s_pipex.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npirard <npirard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 16:15:18 by npirard           #+#    #+#             */
/*   Updated: 2023/12/14 17:35:32 by npirard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pipex.h>
#include <libft.h>

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

void	print_pipex(t_pipex *pipex)
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
