/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npirard <npirard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 18:25:14 by npirard           #+#    #+#             */
/*   Updated: 2023/12/08 11:40:55 by npirard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pipex.h>

char	**command_build(char *input, char **env)
{
	char	**strs;
	char	*command_path;

	strs = ft_split(input, ' ');
	if (!strs)
		return (alloc_error());
	if (strs[0])
	{
		command_path = command_find_path(strs[0], env);
		if (!command_path)
		{
			error_input(3);
			return (ft_free_strs(strs));
		}
		free(strs[0]);
		strs[0] = command_path;
	}
	return (strs);
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
