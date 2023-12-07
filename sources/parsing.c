/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npirard <npirard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 15:46:33 by npirard           #+#    #+#             */
/*   Updated: 2023/12/07 18:26:59 by npirard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pipex.h>

static char	***parse_commands(int size, char **argv)
{
	char	***commands;
	int		i;

	commands = malloc(sizeof(char **) * size);
	if (!commands)
		return (NULL);
	i = 0;
	while (i < size)
	{
		commands[i] = build_command(argv[i]);
		if (!commands[i])
			return (commands_free(commands));
		i++;
	}
	return (commands);
}

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

static void	parse_file_out(int *size, char **args, t_pipex *pipex)
{
	if (*size == 1)
		return ;
	if (!ft_strcmp(args[*size - 1], "-a"))
	{
		if (*size == 2)
			exit(error_input(pipex_clear(pipex, 2)));
		pipex->flag_a = true;
		pipex->file_out = ft_strdup(args[*size - 2]);
		size--;
	}
	else
		pipex->file_out = ft_strdup(args[*size - 1]);
	if (!pipex->file_out)
		exit(error_input(pipex_clear(pipex, -1)));
}

static void	parse_args(int size, char **args, t_pipex *pipex)
{
	args = parse_file_in(&size, args, pipex);
	parse_file_out(&size, args, pipex);
	pipex->commands = parse_commands(size, args);
	if (!pipex->commands)
		exit(error_input(pipex_clear(pipex, -1)));
}

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
