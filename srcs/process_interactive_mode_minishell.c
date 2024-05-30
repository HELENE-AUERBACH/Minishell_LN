/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_interactive_mode_minishell.c               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 17:38:11 by hauerbac          #+#    #+#             */
/*   Updated: 2024/05/30 15:29:25 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static void	set_signals_actions(void)
{
	struct sigaction	s1;

	s1.sa_handler = &sigint_handler_in_main;
	sigemptyset(&s1.sa_mask);
	s1.sa_flags = SA_RESTART;
	sigaction(SIGINT, &s1, NULL);
	signal(SIGQUIT, SIG_IGN);
}

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
	if (result == 3)
		display_error("Malloc error\n");
	if (a_line && ft_strlen(a_line) > 0)
		g_exit_status = result;
	return (result);
}	

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
	d->new_files = NULL;
	d->cmd_new_files = NULL;
	return (0);
}

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
