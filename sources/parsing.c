/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npirard <npirard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 15:46:33 by npirard           #+#    #+#             */
/*   Updated: 2023/12/13 09:14:28 by npirard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pipex.h>

/// @brief Retrive given command in argv with the provided numbr of
/// commands.
/// @param size
/// @param argv
/// @param env
/// @return List of commands. Each command is a list of string, starting
/// with the path of the command if existing, and followed by every given
/// args. Last element is ```NULL```. Invalid command or no command are
/// constituted of an empty string followed with ```NULL```.
static char	***parse_commands(int size, char **argv, char **env)
{
	char	***commands;
	int		i;

	commands = malloc(sizeof(char **) * (size + 1));
	if (!commands)
		return (alloc_error());
	commands[size] = NULL;
	i = 0;
	while (i < size)
	{
		commands[i] = command_build(argv[i], env);
		if (!commands[i])
			return (ft_free_strss(commands));
		i++;
	}
	return (commands);
}

/// @brief Check for input file and update size accordingly.
/// If no input file, ```pipex.file_in``` is left ```NULL``` and ```stdin```
/// will be readed. If ```file_in == "here_doc"``` limiter is readed
/// and updated.
/// @param size Pointer toward the number of commands.
/// @param args
/// @param pipex
/// @return Pointer toward the first argument in argv following file input.
static char	**parse_file_in(int *size, char **args, t_pipex *pipex)
{
	if (!ft_strcmp(args[0], "here_doc"))
	{
		pipex->limiter = ft_strdup(args[1]);
		if (!pipex->limiter)
			exit(error_input(pipex_clear(pipex, -1)));
		args += 2;
		*size -= 2;
	}
	else
	{
		pipex->file_in = ft_strdup(args[0]);
		if (!pipex->file_in)
			exit(error_input(pipex_clear(pipex, -1)));
		args++;
		*size -= 1;
	}
	return (args);
}

/// @brief Check for output file and update size accordingly.
/// If no input file, ```pipex.file_out``` is left ```NULL``` and ```stdout```
/// will be readed. If ```-a``` flag is given at the end of input, append
/// flag will be enabled.
/// @param size Pointer toward the number of commands.
/// @param args
/// @param pipex
static void	parse_file_out(int *size, char **args, t_pipex *pipex)
{
	if (!ft_strcmp(args[*size - 1], "-a"))
	{
		if (*size < 3)
			exit(error_input(pipex_clear(pipex, 2)));
		pipex->flag_a = true;
		pipex->file_out = ft_strdup(args[*size - 2]);
		*size -= 2;
	}
	else if (*size == 1)
		return ;
	else
		pipex->file_out = ft_strdup(args[--(*size)]);
	if (!pipex->file_out)
		exit(error_input(pipex_clear(pipex, -1)));
}

/// @brief Update pipex structure with given args.
/// @param size Number of given commands.
/// @param args ```Argv + 1```
/// @param pipex
static void	parse_args(int size, char **args, t_pipex *pipex)
{
	args = parse_file_in(&size, args, pipex);
	parse_file_out(&size, args, pipex);
	pipex->commands = parse_commands(size, args, pipex->env);
	if (!pipex->commands)
		exit(pipex_clear(pipex, -1));
}

/// @brief Build a pipex structure where different value in the input
/// are parsed.
/// @param argc
/// @param argv
/// @param env
/// @return
/// @note Check for :
/// @note - Incorrect number of arguments (< 3 by default or < 4/5 if ```-a```
/// flag is given when parsing ```file_in```.)
/// @note - Non existing file/command or file/command without access rights.
/// In this case, error message will be printed and corresponding attributs
/// in ```pipex``` structure will be left empty.
/// @note Only wrong number of arguments, flag error or will stop the program
/// execution. Wrong command or file errors will be detected and handled but
/// the program will be still executed.
t_pipex	*parse_input(int argc, char **argv, char **env)
{
	t_pipex	*pipex;

	if (argc < MIN_NARG)
		exit(error_input(1));
	pipex = pipex_init();
	if (!pipex)
		exit(error_alloc());
	pipex->env = env;
	parse_args(argc - 1, argv + 1, pipex);
	return (pipex);
}
