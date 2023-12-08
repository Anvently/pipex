/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npirard <npirard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 15:46:52 by npirard           #+#    #+#             */
/*   Updated: 2023/12/08 12:22:42 by npirard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pipex.h>

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
		"file_in", "file_out", "flag_a", "limiter",
		"number of commands");
	while (pipex->commands && pipex->commands[i])
	{
		ft_printf("  %i\n", i);
		ft_print_strs(pipex->commands[i]);
		i++;
	}
}

int	main(int argc, char **argv, char **env)
{
	t_pipex	*pipex;

	pipex = parse_input(argc, argv, env);
	print_pipex(pipex);
	pipex_clear(pipex, 0);
	return (0);
}
