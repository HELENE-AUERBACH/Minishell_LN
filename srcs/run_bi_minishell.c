/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_bi_minishell.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmorice <rmorice@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 15:56:35 by hauerbac          #+#    #+#             */
/*   Updated: 2024/09/25 14:34:29 by rmorice          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* ************************************************************************** */
/*                              check_and_run_bi                              */
/* -------------------------------------------------------------------------- */
/* This function checks if the token is a builtin. If that is the case, the   */
/* relative built function is call.                                           */
/* Inputs :                                                                   */
/*  - t_data *d : a structure that contained infos relative to the shell      */
/*  - t_cmd *bi : a structure that contained datas relative to builtin cmd    */
/*  - int fd2 : the file descriptor of the output                             */
/* Return :                                                                   */
/*  - 0 : if everything goes well                                             */
/*  - int : the error code of the problem encounter                           */
/* ************************************************************************** */
int	check_and_run_bi(t_data *d, t_cmd *bi, int fd2)
{
	if (bi && bi->cmd && bi->cmd[0] != '\0')
	{
		if (ft_strncmp("echo", bi->cmd, 4) == 0)
			return (built_echo(bi->args, fd2));
		if (ft_strncmp("cd", bi->cmd, 2) == 0)
			return (built_cd(&d->envp, &d->envp_size, bi->args, \
						fd2));
		if (ft_strncmp("pwd", bi->cmd, 3) == 0)
			return (built_pwd(bi->args, fd2));
		if (ft_strncmp("export", bi->cmd, 6) == 0)
			return (built_export(&d->envp, &d->envp_size, \
						bi->args, fd2));
		if (ft_strncmp("unset", bi->cmd, 5) == 0)
			return (built_unset(&d->envp, &d->envp_size, bi->args));
		if (ft_strncmp("env", bi->cmd, 3) == 0)
			return (built_env(d->envp, bi->args, fd2));
		if (ft_strncmp("exit", bi->cmd, 4) == 0)
			return (built_exit(d, bi->args));
	}
	else if (!(bi && bi->cmd))
		display_err_with_prefix("(null)", " command not found\n");
	else
		display_err_with_prefix(bi->cmd, " command not found\n");
	return (127);
}

/* ************************************************************************** */
/*                     set_signals_actions_in_fork_for_bi                     */
/* -------------------------------------------------------------------------- */
/* This function changes the action associated to signal SIGINT and SIGQUIT   */
/* (cf. set_signals_actions_in_fork). Then if an array of paths exist then it */
/* is freed                                                                   */
/* Inputs :                                                                   */
/*  - t_dll *lst : a double list associated to tokens (struct t_token)        */
/*  - t_token *t : a structure that contained datas about the current token   */
/*  - t_list *current : current token that we are treated (here a built_in)   */
/* Return :                                                                   */
/*  - None                                                                    */
/* ************************************************************************** */
static void	set_signals_actions_in_fork_for_bi(t_dll *lst, t_token *t,
			t_list *current)
{
	set_signals_actions_in_fork(lst, current);
	if (t->cmd_d->paths_tab)
		free_tab(&t->cmd_d->paths_tab);
}

/* ************************************************************************** */
/*                            run_bi_without_fork                             */
/* -------------------------------------------------------------------------- */
/* This function opens the wanted files with the expencted options then it    */
/* checks if the command is a builtin (echo, cd, pwd, export, unset, env,     */
/* exit) and, if that is the case, the builtin function is run                */
/* Once the builtin finished running then opened files are closed             */
/* Input :                                                                    */
/*  - t_data *d : a structure that contained infos relative to the shell      */
/* Return :                                                                   */
/*  - int : the return value (error code or exit status)                      */
/* ************************************************************************** */
int	run_bi_without_fork(t_data *d, t_token *t)
{
	int	result;
	int	fd2;

	result = 0;
	if (!(d && d->envp && t && t->cmd_d))
		return (-1);
	if (files_open(t) < 0)
		return (-2);
	if (t->cmd_d->fd2 != -1)
		fd2 = t->cmd_d->fd2;
	else
		fd2 = 1;
	result = check_and_run_bi(d, t->cmd_d, fd2);
	if (t->cmd_d->fd1 != -1)
		close_in_file_and_free_file_name(t->cmd_d);
	if (t->cmd_d->fd2 != -1)
		close_out_file_and_free_file_name(t->cmd_d);
	return (result);
}

/* ************************************************************************** */
/*                            free_for_bi_in_fork                             */
/* -------------------------------------------------------------------------- */
/* This function frees every lists relative to cmds and files as well as      */
/* everything that can be freed inside d                                      */
/* rq : if we are in interactive mode then the history is cleared as well     */
/* Input :                                                                    */
/*  - t_data *d : a structure that contained infos relative to the shell      */
/* Return :                                                                   */
/*  - None                                                                    */
/* ************************************************************************** */
static void	free_for_bi_in_fork(t_data *d)
{
	empty_list(&d->cmds);
	empty_list(&d->new_files);
	empty_list(&d->cmd_new_files);
	if (is_in_interactive_mode())
		rl_clear_history();
	free_data(d);
}

/* ************************************************************************** */
/*                               run_bi_in_fork                               */
/* -------------------------------------------------------------------------- */
/* This function clones the calling process and if no error occured it change */
/* the action associated to the signals SIGINT and SIGQUIT in the child       */
/* process. The input and output are redirected if needed.                    */
/* Then the function associated to the builtin token is call.                 */
/* If an error occured then an error message is display and we free and       */
/* closed the pipe descriptors and file descriptors. We also clear everything */
/* related to the command and files                                           */
/* In the parent proccess we only close the file descriptors and ds.          */
/* Inputs :                                                                   */
/*  - t_data *d : a structure that contained infos relative to the shell      */
/*  - t_token *t : a structure that contained datas about the current token   */
/*  - int ds[3] : an array that contained fds associated to pipe              */
/*  - t_list *current : current token that we are treated (here a built_in)   */
/* Return :                                                                   */
/*  - None                                                                    */
/* ************************************************************************** */
void	run_bi_in_fork(t_data *d, t_token *t, int ds[3], t_list *current)
{
	const int	is_piped = (ds[0] != -1 && ds[1] != -1);
	int			result;

	t->cmd_d->pid = fork();
	if ((int) t->cmd_d->pid == -1)
		perr_cds(d, "Fork error", ds, is_piped);
	if ((int) t->cmd_d->pid == 0)
	{
		set_signals_actions_in_fork_for_bi(d->lst, t, current);
		if (t->cmd_d->is_in_piped == 1 && ds[2] != -1
			&& t->cmd_d->fd1 == -1 && dup2(ds[2], 0) == -1)
			perr_cds(d, DUP_PREV_ERR, ds, is_piped);
		if (t->cmd_d->fd1 != -1 && dup2(t->cmd_d->fd1, 0) == -1)
			perr_cds(d, REDIR_INPUT_ERR, ds, is_piped);
		if (t->cmd_d->is_out_piped == 1 && t->cmd_d->fd2 == -1
			&& dup2(ds[1], 1) == -1)
			perr_cds(d, DUP_1_ERR, ds, is_piped);
		if (t->cmd_d->fd2 != -1 && dup2(t->cmd_d->fd2, 1) == -1)
			perr_cds(d, REDIR_OUTPUT_ERR, ds, is_piped);
		close_descrs_with_a_possible_exit(d, t, ds, is_piped);
		result = check_and_run_bi(d, t->cmd_d, 1);
		free_for_bi_in_fork(d);
		exit (result);
	}
	close_ds_in_parent(t, ds, is_piped);
}
