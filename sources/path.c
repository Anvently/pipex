/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npirard <npirard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 10:33:36 by npirard           #+#    #+#             */
/*   Updated: 2023/12/08 13:16:10 by npirard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pipex.h>
#include <unistd.h>

/// @brief Allocate and return the concatenation of path and command, adding a
/// '/' in between.
/// @param path Null terminated string
/// @param command Null terminated string
/// @return Address of allocated string
/// NULL if allocation fails.
static char	*path_join(char const *path, char const *command)
{
	char	*buffer;
	size_t	size;
	size_t	len_path;
	size_t	len_command;

	len_path = ft_strlen(path);
	len_command = ft_strlen(command);
	if (!path || !command)
		return (NULL);
	size = len_path + len_command + 2;
	buffer = malloc(size);
	if (!buffer)
		return (NULL);
	ft_strlcpy(buffer, path, size);
	buffer[len_path] = '/';
	buffer[len_path + 1] = '\0';
	ft_strlcat(buffer, command, size);
	return (buffer);
}

static char	*build_path(char *command, char **path_strs)
{
	char	*path;

	while (*path_strs)
	{
		path = path_join(*path_strs, command);
		if (!path)
			return (alloc_error());
		if (path_exist(path, false))
		{
			if (path_is_exec(path, true))
				return (path);
			else
			{
				free(path);
				return (NULL);
			}
		}
		free(path);
		path_strs++;
	}
	error_path("Command not found: ", command);
	return (NULL);
}

static char	**split_path(char **env)
{
	char	*sub_path;
	char	**path_strs;

	path_strs = NULL;
	while (env && ft_strncmp(*env, "PATH=", 5))
		env++;
	if (env)
	{
		sub_path = ft_substr(*env, 5, ft_strlen(*env) - 5);
		if (!sub_path)
			return (alloc_error());
		path_strs = ft_split(sub_path, ':');
		free(sub_path);
		if (!path_strs)
			return (alloc_error());
	}
	else
		error_input(3);
	return (path_strs);
}

char	*command_find_path(char *command, char **env)
{
	char		**path_strs;
	char		*path_abs;

	if (command_is_path(command))
	{
		path_abs = ft_strdup(command);
		if (!path_abs)
			return (alloc_error());
	}
	else
	{
		path_strs = split_path(env);
		if (!path_strs)
			return (NULL);
		path_abs = build_path(command, path_strs);
		ft_free_strs(path_strs);
		return (path_abs);
	}
	if (path_exist(path_abs, true) && path_is_exec(path_abs, true))
		return (path_abs);
	return (NULL);
}
