/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npirard <npirard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 15:46:52 by npirard           #+#    #+#             */
/*   Updated: 2023/12/07 17:26:09 by npirard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pipex.h>

int	main(int argc, char **argv, char **env)
{
	t_pipex	*pipex;

	pipex = parse_input(argc, argv, env);
	pipex_clear(pipex, 0);
	return (0);
}
