/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_check.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npirard <npirard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 09:49:17 by npirard           #+#    #+#             */
/*   Updated: 2023/12/08 15:00:12 by npirard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pipex.h>
#include <unistd.h>

/// @brief Check for read access to ```file_in``` and write access to
/// ```file_out```. ```stdin``` and ```stdout``` (```NULL```) are ignored.
/// stdin or stdout are
/// @param file_in
/// @param file_out
/// @return ```true``` if access are ok, else ```false```.
bool	check_file_access(char *file_in, char *file_out)
{
	if (file_in)
	{
		if (!path_exist(file_in, true))
			return (false);
		if (access(file_in, R_OK) != 0)
		{
			error_path("Permission denied: ", file_in);
			return (false);
		}
	}
	if (file_out)
	{
		if (path_exist(file_out, false) && access(file_out, W_OK) != 0)
		{
			error_path("Permission denied: ", file_out);
			return (false);
		}
	}
	return (true);
}

/// @brief Check if the command is already a path. Meaning it starts with a
/// ```.```, a ```~``` or a ```/```
/// @param command
/// @return ```true``` if command is a path, else ```false```.
bool	command_is_path(char *command)
{
	if (ft_strchr("./~", command[0]))
		return (true);
	return (false);
}

bool	path_exist(char *path, bool printerr)
{
	if (access(path, F_OK) == 0)
		return (true);
	if (printerr)
		error_path("No such file or directory: ", path);
	return (false);
}

bool	path_is_exec(char *path, bool printerr)
{
	if (access(path, X_OK) == 0)
		return (true);
	if (printerr)
		error_path("Permission denied: ", path);
	return (false);
}
