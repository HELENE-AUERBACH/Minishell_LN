/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_cmds_minishell.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 15:27:39 by hauerbac          #+#    #+#             */
/*   Updated: 2024/06/05 15:26:05 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	add_to_cmds_list(t_data *d, t_token *t_cmdbi)
{
	int		result;
	t_list	*lst_new;

	result = 0;
	if (!d)
		return (-2);
	if (!t_cmdbi || !t_cmdbi->cmd_d || !t_cmdbi->cmd_d->args
		|| !t_cmdbi->cmd_d->args[0])
		return (display_error("(null) : command not found\n"), 127);
	if (t_cmdbi->cmd_d->args && t_cmdbi->cmd_d->args[0])
		ft_strlcpy(t_cmdbi->cmd_d->cmd, t_cmdbi->cmd_d->args[0], \
				1 + ft_strlen(t_cmdbi->cmd_d->args[0]));
	if (!t_cmdbi->cmd_d->cmd)
		return (display_error("(null) : command not found\n"), 127);
	if (t_cmdbi->cmd_d->cmd[0] == '\0'
		|| t_cmdbi->cmd_d->args[0][0] == '\0')
		return (display_error("  : command not found\n"), 127);
	lst_new = ft_lstnew((void *) t_cmdbi);
	if (!lst_new)
		return (3);
	ft_lstadd_back(&d->cmds, lst_new);
	lst_new = NULL;
	d->nb_cmds++;
	t_cmdbi->cmd_d->order = d->nb_cmds;
	return (result);
}

static int	open_pipe_and_run_command(t_data *d, t_list *current,
			int *is_piped, int *ds)
{
	t_token	*t;

	t = (t_token *) current->content;
	if (t && t->cmd_d && t->cmd_d->paths_tab)
		free_tab(&t->cmd_d->paths_tab);
	t->cmd_d->paths_tab = get_paths(d->envp);
	if (t && t->cmd_d && !t->cmd_d->paths_tab)
		return (-3);
	if (files_open(t) < 0)
		return (-5);
	if (t->cmd_d->followed_by_op != PIPE && *is_piped == 0 && t->type == BI)
		return (run_bi_without_fork(d, t));
	ds[2] = ds[0];
	ds[0] = -1;
	if (t->cmd_d->is_out_piped == 1 && pipe(ds) == -1)
		return (perror("Pipe error"), close_descriptors(ds, 0), -4);
	if (t->cmd_d->is_out_piped == 1)
		*is_piped = 1;
	if (t->type == COMMAND)
		run_command(d, t, ds, current);
	else if (t->type == BI)
		run_bi_in_fork(d, t, ds, current);
	return (0);
}

static int	open_pipes_and_run_commands(t_data *d, t_list **start,
			t_list **end)
{
	int		is_piped;
	int		*ds;
	t_list	*current;
	t_token	*t;

	is_piped = 0;
	ds = (int [3]){-1, -1, -1};
	current = *start;
	t = (t_token *) current->content;
	while (current && t && t->cmd_d)
	{
		if (open_pipe_and_run_command(d, current, &is_piped, ds) < 0)
			return (-1);
		close_files_and_free_files_names_without_unlink(t->cmd_d);
		current = current->next;
		*start = current;
		*end = current;
		if (t->cmd_d->followed_by_op != PIPE)
			break ;
		t = NULL;
		if (current && current->content)
			t = (t_token *) current->content;
	}
	return (close_descriptors(ds, is_piped), 0);
}

static int	run_subset_of_commands(t_data *d, t_list **start, int wstatus,
			pid_t w)
{
	t_list	*current;
	t_list	*end;
	t_token	*t;

	current = *start;
	end = NULL;
	if (open_pipes_and_run_commands(d, start, &end) < 0)
		return (EXIT_FAILURE);
	t = (t_token *) current->content;
	while (current && t && t->cmd_d && (int) t->cmd_d->pid > 0)
	{
		w = waitpid(t->cmd_d->pid, &wstatus, 0);
		current = current->next;
		empty_dll_before_cur(d->lst, current, del_el_content);
		t = NULL;
		if (current && current->content && current != end)
			t = (t_token *) current->content;
	}
	if (w < 0)
		return (perror("waitpid failed"), EXIT_FAILURE);
	if (WIFEXITED(wstatus))
		return (WEXITSTATUS(wstatus));
	if (WIFSIGNALED(wstatus))
		return (128 + WTERMSIG(wstatus));
	return (display_error("Last cmd nor exited, nor signaled\n"), 1);
}

int	run_commands(t_data *d)
{
	t_list	*start;
	int		result;
	int		wstatus;
	pid_t	w;

	if (!(d && d->cmds && d->nb_cmds > 0))
		return (-2);
	start = d->cmds;
	if (!start->content)
		return (-3);
	if (d->nb_cmds == 1 && ((t_token *) start->content)->type == BI)
		return (run_bi_without_fork(d, ((t_token *) start->content)));
	result = 0;
	wstatus = 0;
	w = 0;
	while (start && start->content)
		result = run_subset_of_commands(d, &start, wstatus, w);
	return (result);
}
