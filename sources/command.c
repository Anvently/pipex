/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npirard <npirard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 18:25:14 by npirard           #+#    #+#             */
/*   Updated: 2023/12/14 11:15:55 by npirard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pipex.h>
#include <libft.h>

/// @brief Parse given command spliting its name (looking for
/// a path if necessary) and its arguments.
/// @param input
/// @param env
/// @return List of string starting with command path and followed by
/// given arguments. Ends with ```NULL```. If command doesn't exist or path
/// is invalid, first string will be an empty string.
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
			return (ft_free_strs(strs));
		free(strs[0]);
		strs[0] = command_path;
	}
	return (strs);
}
