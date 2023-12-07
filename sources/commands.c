/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npirard <npirard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 18:25:14 by npirard           #+#    #+#             */
/*   Updated: 2023/12/07 18:26:26 by npirard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pipex.h>

char	**build_command(char *input)
{

}

void	*commands_free(char ***commands)
{
	int	i;
	int	j;

	i = 0;
	while (commands && commands[i])
	{
		j = 0;
		while (commands[i] && commands[i][j])
			free(commands[i][j++]);
		free(commands[i++]);
	}
	free(commands);
	return (NULL);
}
