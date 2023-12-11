/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npirard <npirard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 10:33:36 by npirard           #+#    #+#             */
/*   Updated: 2023/12/11 18:47:29 by npirard          ###   ########.fr       */
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

/// @brief Return the path of a command if it can be found in one of
/// ```path_strs```'s path and if the file is executable. Else an error is
/// printed and an empty string is returned.
/// @param command
/// @param path_strs
/// @return Path of given command, empty string is no path is found or if
/// path is not executable. ```NULL``` if allocation error.
static char	*build_path(char *command, char **path_strs)
{
	char	*path;

	while (*path_strs)
	{
		path = path_join(*path_strs++, command);
		if (!path)
			return (alloc_error());
		if (path_exist(path, false))
		{
			if (path_is_exec(path, true))
				return (path);
			else
			{
				free(path);
				break ;
			}
		}
		free(path);
	}
	if (!*path_strs)
		error_path("Command not found: ", command);
	path = ft_strdup("");
	if (!path)
		return (alloc_error());
	return (path);
}

/// @brief Search for ```PATH``` variable in ```env``` and split it into
/// a list of strings.
/// @param env
/// @return List of strings for each path inside ```PATH``` variable.
/// ```NULL``` if allocation error or if no ```PATH``` variable is found in env.
static char	**split_path(char **env)
{
	char	*sub_path;
	char	**path_strs;

	path_strs = NULL;
	while (env && *env && ft_strncmp(*env, "PATH=", 5))
		env++;
	if (env && *env)
	{
		sub_path = ft_substr(*env, 5, ft_strlen(*env) - 5);
		if (!sub_path)
			return (alloc_error());
		path_strs = ft_split(sub_path, ':');
		free(sub_path);
	}
	else
		path_strs = ft_split("", ' ');
	if (!path_strs)
		return (alloc_error());
	else if (path_strs[0] == NULL)
		error_input(3);
	return (path_strs);
}

/// @brief Check if given command is a path or can be found in PATH variable,
/// and if it can be executed. For invalid command, error message are printed.
/// @param command
/// @param env
/// @return Path of the command to execute if correct, empty string if command
/// is invalid or ```NULL``` if allocation error.
char	*command_find_path(char *command, char **env)
{
	char		**path_strs;
	char		*path_abs;

	if (command_is_path(command) || !ft_strcmp("", command))
	{
		if (!ft_strcmp("", command)
			|| (path_exist(command, true) && path_is_exec(command, true)))
			path_abs = ft_strdup(command);
		else
			path_abs = ft_strdup("");
		if (!path_abs)
			return (alloc_error());
		return (path_abs);
	}
	path_strs = split_path(env);
	if (!path_strs)
		return (NULL);
	path_abs = build_path(command, path_strs);
	ft_free_strs(path_strs);
	return (path_abs);
}
