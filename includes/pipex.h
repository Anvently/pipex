/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npirard <npirard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 15:47:14 by npirard           #+#    #+#             */
/*   Updated: 2023/12/14 18:00:17 by npirard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <stdbool.h>

# define MIN_NARG 3
# define HERE_DOC_TMP_PATH "/tmp/pipex_tmp"

/// @brief
/// @param file_in File to take input from. If ```NULL``` input will
/// be ```stdin```.
/// @param file_out File to print the result to. If ```NULL``` output will be
/// ```stdout```.
/// @param flag_a If ```-a``` is given, output will be added to the end of file
/// instead of overwriting it.
/// @param limiter Used in combination with ```here_doc``` as input file. Define
/// a limiter where pipex will stop writing in output when limiter is
/// encountered.
/// @param commands List of commands to execute in order. Match with ```argv```
/// standard format.
/// @param env Pointer to env.
typedef struct s_pipex
{
	char	*file_in;
	char	*file_out;
	bool	flag_a;
	char	*limiter;
	char	***commands;
	char	**env;
}			t_pipex;

/* -------------------------------------------------------------------------- */
/*                               PIPEX STRUCTURE                              */
/* -------------------------------------------------------------------------- */

t_pipex	*pipex_init(void);
int		pipex_clear(t_pipex *pipex, int error);
void	print_pipex(t_pipex *pipex);

/* -------------------------------------------------------------------------- */
/*                                    ERROR                                   */
/* -------------------------------------------------------------------------- */

int		error(char *error_msg, int error);
int		error_path(char *msg, char *path);
int		error_input(int error);
int		error_alloc(void);
int		error_close(int fd, int error);

/* -------------------------------------------------------------------------- */
/*                                   PARSING                                  */
/* -------------------------------------------------------------------------- */

t_pipex	*parse_input(int argc, char **argv, char **env);

/* -------------------------------- COMMANDS -------------------------------- */

char	**command_build(char *input, char **env);
char	*command_find_path(char *command, char **env);

/* ------------------------------- PATH CHECK ------------------------------- */

bool	command_is_path(char *command);
bool	path_exist(char *path, bool printerr);
bool	path_is_exec(char *path, bool printerr);
bool	check_file_access(char *file_in, char *file_out);

/* -------------------------------- PIPEX -------------------------------- */

int		exec_cmds(t_pipex *pipex, char **env, int *old_fd, int i_cmd);
int		here_doc(char *limiter);

/* ------------------------------- PIPEX_UTILS ------------------------------ */

int		fd_to_fd(int fd_in, int fd_out);

#endif
