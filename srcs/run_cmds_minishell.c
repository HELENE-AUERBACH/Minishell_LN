/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_cmds_minishell.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmorice <rmorice@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 15:27:39 by hauerbac          #+#    #+#             */
/*   Updated: 2024/09/13 11:29:16 by rmorice          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* ************************************************************************** */
/*                              add_to_cmds_list                              */
/* -------------------------------------------------------------------------- */
/* This function copies and adds the current command at the end of d->cmds.   */
/* The number of command is then update and the order of priority of this     */
/* command is specified                                                       */
/* Inputs :                                                                   */
/*  - t_data *d : a structure that contained infos relative to the shell      */
/*  - t_token *t_cmdbi   */
/* Return :                                                                   */
/*  - 0 : if everything goes well                                             */
/*  - int : the error code of the problem encounter                           */
/* ************************************************************************** */
// from what I see we only copy the command name, but we need to link it to the args
// or did I misread the type of the copy element and it is the array of string that
// contained cmd_name && args ???
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

/* ************************************************************************** */
/*                         open_pipe_and_run_command                          */
/* -------------------------------------------------------------------------- */
/* This function opens the files needed, opens a pipe if needed amd run the   */
/* current command or builtin                                                 */
/* If the token is of type builtin then, if it isn't piped and isn't followed */
/* by a pipe token then the builtin is run without forking the system.        */
/* Otherwise, the builtin is run inside a child process                       */
/* Inputs :                                                                   */
/*  - t_data *d : a structure that contained infos relative to the shell      */
/*  - t_list *current      */
/*  - int *is_piped      */
/*  - int *ds      */
/* Return :                                                                   */
/*  - 0 : if everything goes well                                             */
/*  - int : the error code of the problem encounter                           */
/* ************************************************************************** */
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

/* ************************************************************************** */
/*                        open_pipes_and_run_commands                         */
/* -------------------------------------------------------------------------- */
/* This function opens every needed pipe(s) and run every command encountered */
/* (token of type command or builtin) until an operator that isn't a pipe is  */
/* met                                                                        */
/* The files opened are closed and every associated variable is free, if      */
/* needed, and reinitialised                                                  */
/* The pipe descriptors are closed once no longer needed                      */
/* Inputs :                                                                   */
/*  - t_data *d : a structure that contained infos relative to the shell      */
/*  - t_list **start : the first element to take into account                 */
/*  - t_list **end : the element where we stopped to run the token            */
/* Return :                                                                   */
/*  - 0 : if everything goes well                                             */
/*  - int : the error code of the problem encounter                           */
/* ************************************************************************** */
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

/* ************************************************************************** */
/*                           run_subset_of_commands                           */
/* -------------------------------------------------------------------------- */
/* This function runs every commands that are part of the same sub-set. That  */
/* mean every command separated by a pipe or a single command. A sub-set is   */
/* delimited by the end of the command line or an operator that isn't a pipe. */
/* rq : if the process as been fork then we wait for every child process to   */
/* finished and we free the element that has been treated                     */
/* Inputs :                                                                   */
/*  - t_data *d : a structure that contained infos relative to the shell      */
/*  - t_list **start      */
/*  - int wstatus      */
/*  - pid_t w      */
/* Return :                                                                   */
/*  - ??? ( what is the "everything goes well" exit value ?)   */
/*  - int : the error code of the problem encounter                           */
/* ************************************************************************** */
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

/* ************************************************************************** */
/*                                run_commands                                */
/* -------------------------------------------------------------------------- */
/* This function checks that we have some commands to run. If that is the     */
/* case then the builtin function associated to the token or the subset of    */
/* commands that we encountered is run                                        */
/* Input :                                                                    */
/*  - t_data *d : a structure that contained infos relative to the shell      */
/* Return :                                                                   */
/*  - ??? ( what is the "everything goes well" exit value ?)   */
/*  - int : the error code of the problem encounter                           */
/* ************************************************************************** */
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
