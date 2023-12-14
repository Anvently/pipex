/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npirard <npirard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 11:10:01 by npirard           #+#    #+#             */
/*   Updated: 2023/12/14 18:13:58 by npirard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pipex.h>
#include <unistd.h>

int	main(int argc, char **argv, char **env)
{
	t_pipex	*pipex;
	int		status;

	pipex = parse_input(argc, argv, env);
	status = exec_cmds(pipex, env, NULL, 0);
	pipex_clear(pipex, 0);
	return (status);
}
