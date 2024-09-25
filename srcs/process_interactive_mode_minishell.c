/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_interactive_mode_minishell.c               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmorice <rmorice@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 17:38:11 by hauerbac          #+#    #+#             */
/*   Updated: 2024/09/25 10:59:50 by rmorice          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* ************************************************************************** */
/*                                 empty_list                                 */
/* -------------------------------------------------------------------------- */
/* This function frees the elements that have to be free from the t_list and  */
/* makes pointers point to NULL for each node.                                */
/* Input :                                                                    */
/*  - t_list **lst : the list to empty                                        */
/* Return :                                                                   */
/*  - None                                                                    */
/* ************************************************************************** */
void	empty_list(t_list **lst)
{
	t_list	*current;
	t_list	*next;

	if (lst)
	{
		current = *lst;
		while (current)
		{
			next = current->next;
			current->next = NULL;
			current->content = NULL;
			free(current);
			current = next;
		}
		*lst = NULL;
	}
}

/* ************************************************************************** */
/*                            set_signals_actions                             */
/* -------------------------------------------------------------------------- */
/* This function changes the action associated to signal SIGINT to the one    */
/* associated to s1                                                           */
/* we also specified that the signal SIGQUIT should be ignored                */
/* To do so it specified that s1.sa_handler should displays a newline and     */
/* tells the system that we have move to a newline with (g_exit_status = 130) */
/* rq : every signals given by s1.sa_mask is emptied before sigaction call    */
/* Input :                                                                    */
/*  - None                                                                    */
/* Return :                                                                   */
/*  - None                                                                    */
/* ************************************************************************** */
void	set_signals_actions(void)
{
	struct sigaction	s1;

	s1.sa_handler = &sigint_handler_in_main;
	sigemptyset(&s1.sa_mask);
	s1.sa_flags = SA_RESTART;
	sigaction(SIGINT, &s1, NULL);
	signal(SIGQUIT, SIG_IGN);
}

/* ************************************************************************** */
/*                               process_a_line                               */
/* -------------------------------------------------------------------------- */
/* This function process the line (splits it into tokens, determines the role */
/* of each token, do redirections and extension and runs builtin functions or */
/* sub-set of commands)                                                       */
/* rq : the overall return code is update if needed                           */
/* Inputs :                                                                   */
/*  - char *a_line : the line to analyse and process                          */
/*  - t_data *d : a structure that contained infos relative to the shell      */
/* Return :                                                                   */
/*  - 0 : if everything goes well                                             */
/*  - int : the error code of the problem encounter                           */
/* ************************************************************************** */
int	process_a_line(char *a_line, t_data *d)
{
	int	result;

	d->nb_cmds = 0;
	result = split_in_tokens(a_line, d->lst);
	if (result == 0 && d->lst && d->lst->size > 0)
	{
		result = dll_iter_with_param(d->lst, d,
				perform_expansions);
		if (result == 0)
		{
			result = parse_tokens(d);
			if (result == 0 && d->nb_cmds > 0)
				result = run_commands(d);
		}
	}
	if (result == 3 || result == -3)
		display_error("Malloc error\n");
	if (a_line && ft_strlen(a_line) > 0)
	{
		if (result == 0 && g_exit_status != 0)
			d->return_code = g_exit_status;
		else
			d->return_code = result;
	}
	return (result);
}

/* ************************************************************************** */
/*                            init_minishell_data                             */
/* -------------------------------------------------------------------------- */
/* This function initialises the t_data structure received in arguments       */
/* rq : g_exit_status is set by the signal, nb_cmds = 0, every t_dll and      */
/* t_list are set to NULL and the return_code is set to 0                     */
/* Input :                                                                    */
/*  - t_data *d : the structure to initialise                                 */
/* Return :                                                                   */
/*  - 0 : if everything goes well                                             */
/*  - 1 othewrwise                                                            */
/* ************************************************************************** */
static int	init_minishell_data(t_data *d)
{
	if (!d)
		return (EXIT_FAILURE);
	set_signals_actions();
	d->lst = new_empty_dll();
	if (!d->lst)
		return (EXIT_FAILURE);
	d->cmds = NULL;
	d->nb_cmds = 0;
	d->return_code = 0;
	d->new_files = NULL;
	d->cmd_new_files = NULL;
	return (0);
}

/* ************************************************************************** */
/*                    process_interactive_mode(t_data *d)                     */
/* -------------------------------------------------------------------------- */
/* This function reads the line that the user send in input, it adds it to    */
/* historic, then it process the line (determines the role of each token, do  */
/* redirections and extension and launch the commands with their arguments)   */
/* Then everything is clear and free                                          */
/* Input :                                                                    */
/*  - t_data *d : the structure that contained infos relative to the shell    */
/* Return :                                                                   */
/*  - 0 : if everything goes well                                             */
/*  - int : the error code of the problem encounter                           */
/* ************************************************************************** */
int	process_interactive_mode(t_data *d)
{
	int		result;
	char	*a_line;

	if (init_minishell_data(d) != 0)
		return (EXIT_FAILURE);
	result = 0;
	a_line = "Start";
	while (a_line)
	{
		a_line = readline("Minishell > ");
		if (a_line == NULL && g_exit_status != 130)
			return (write(1, "exit\n", 5), rl_clear_history(),
				free_data(d), 0);
		if (a_line == NULL)
			break ;
		add_history(a_line);
		result = process_a_line(a_line, d);
		free(a_line);
		a_line = "Start";
		empty_list(&d->cmds);
		empty_list(&d->new_files);
		empty_list(&d->cmd_new_files);
		empty_dll(d->lst, del_el_content);
	}
	return (rl_clear_history(), free_data(d), result);
}
