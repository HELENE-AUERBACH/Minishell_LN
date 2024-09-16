/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmorice <rmorice@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 16:38:35 by hauerbac          #+#    #+#             */
/*   Updated: 2024/09/13 11:58:02 by rmorice          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* ************************************************************************** */
/*                            init_minishell_data                             */
/* -------------------------------------------------------------------------- */
/* This function initialises the t_data structure received in arguments       */
/* Input :                                                                    */
/*  - t_data *d : the structure to initialise                                 */
/* Return :                                                                   */
/*  - 0 : if everything goes well                                             */
/*  - 1 othewrwise                                                            */
/* ************************************************************************** */
static int	init_minishell_data(t_data *d)
{
	g_exit_status = 0;
	if (!d)
		return (EXIT_FAILURE);
	d->lst = NULL;
	d->cmds = NULL;
	d->nb_cmds = -1;
	d->return_code = 0;
	d->new_files = NULL;
	d->cmd_new_files = NULL;
	d->envp = NULL;
	d->envp_size = -1;
	return (0);
}

/* ************************************************************************** */
/*                           is_in_interactive_mode                           */
/* -------------------------------------------------------------------------- */
/* This function checks if we are in interactive mode or not (interactive     */
/* mode means that the user is allow to type commands and will receive output */
/* from those commands. Non-interactive mode mean that a script is launch)    */
/* arguments                                                                  */
/* Input :                                                                    */
/*  - None                                                                    */
/* Return :                                                                   */
/*  - 0 : if we are not in interactive mode                                   */
/*  - 1 : if we are in interactive mode                                       */
/* ************************************************************************** */
int	is_in_interactive_mode(void)
{
	return (isatty(STDIN_FILENO) && isatty(STDERR_FILENO));
}

int	g_exit_status;

/* ************************************************************************** */
/*                              main - minishell                              */
/* -------------------------------------------------------------------------- */
/* This function displays a prompt and waits for a command. Then it searched  */
/* and launched the right executable. Once the command line has been          */
/* processed and the result has been displayed or saved where expected a new  */
/* line prompt is display and the function waits again, etc.                  */
/* rq : a working history is kept, quoting is taking into account as well as  */
/* redirections, pipes, environmental variables.                              */
/* rq2 : interactive and non interactive mode has been implemented            */
/* rq3 : some functions builtins as been implemented                          */
/* Inputs :                                                                   */
/*  - const int argc : the number of arguments received (contained in argv)   */
/*  - char **argv : the list of arguments given in input                      */
/*  - char **envp : the list of the environment datas                         */
/* Return :                                                                   */
/*  - 0 : if everything goes well                                             */
/*  - int : the error code of the problem encounter                           */
/* ************************************************************************** */
int	main(const int argc, char **argv, char **envp)
{
	int		fd;
	int		result;
	t_data	d;

	init_minishell_data(&d);
	copy_environment(&d, envp);
	if (d.envp == NULL)
		return (display_error("Environment copy error\n"), 1);
	if (is_in_interactive_mode() && argc == 1)
		return (process_interactive_mode(&d));
	fd = 0;
	if (argc == 2)
	{
		fd = open(argv[1], O_RDONLY);
		if (fd == -1)
			return (perror("Input file open"), free_data(&d), 1);
	}
	result = 1;
	if (argc <= 2)
		result = process_non_interactive_mode(fd, &d);
	else if (argc == 3 && argv[1] && argv[1][0] == '-' && argv[1][1] == 'c')
		return (process_non_interactive_mode_with_c_opt(argv[2], &d));
	if (fd != 0 && close(fd) == -1)
		return (perror("Input file close"), free_data(&d), 1);
	return (free_data(&d), result);
}
