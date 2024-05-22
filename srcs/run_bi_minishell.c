/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_bi_minishell.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbocktor <jbocktor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 13:09:29 by jbocktor          #+#    #+#             */
/*   Updated: 2024/05/21 16:44:35 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_and_run_bi(char ***envp, int *envp_size, t_cmd *bi, int fd2)
{
	*envp = *envp;
	*envp_size = *envp_size;
	if (bi && bi->cmd && bi->cmd[0] != '\0')
	{
		if (ft_strncmp("echo", bi->cmd, 4) == 0)
			return (built_echo(bi->args, fd2));
		if (ft_strncmp("cd", bi->cmd, 2) == 0)
			return (built_cd(envp, envp_size, bi->args));
		if (ft_strncmp("pwd", bi->cmd, 3) == 0)
			return (built_pwd(bi->args));
		if (ft_strncmp("export", bi->cmd, 6) == 0)
			return (built_export(envp, envp_size, bi->args, fd2));
		if (ft_strncmp("unset", bi->cmd, 5) == 0)
			return (built_unset(envp, envp_size, bi->args));
		if (ft_strncmp("env", bi->cmd, 3) == 0)
			return (built_env(*envp, bi->args, fd2));
		if (ft_strncmp("exit", bi->cmd, 4) == 0)
			return (built_exit(bi->args));
	}
	else if (!(bi && bi->cmd))
		display_err_with_prefix("(null)", " command not found\n");
	else
		display_err_with_prefix(bi->cmd, " command not found\n");
	return (127);
}

static void	set_signals_actions_in_fork_for_bi(t_dll *lst, t_token *t,
			t_list *current)
{
	set_signals_actions_in_fork(lst, current);
	if (t->cmd_d->paths_tab)
		free_tab(&t->cmd_d->paths_tab);
}

int	run_bi_without_fork(char ***envp, int *envp_size, t_token *t)
{
	int	result;
	int	fd2;

	result = 0;
	if (!(envp && *envp && envp_size && t && t->cmd_d))
		return (-1);
	if (files_open(t) < 0)
		return (-2);
	if (t->cmd_d->fd2 != -1)
		fd2 = t->cmd_d->fd2;
	else
		fd2 = 1;
	result = check_and_run_bi(envp, envp_size, t->cmd_d, fd2);
	if (t->cmd_d->fd1 != -1)
		close_in_file_and_free_file_name(t->cmd_d);
	if (t->cmd_d->fd2 != -1)
		close_out_file_and_free_file_name(t->cmd_d);
	return (result);
}

static void	free_for_bi_in_fork(t_data *d)
{
	empty_list(&d->cmds);
	empty_list(&d->new_files);
	empty_list(&d->cmd_new_files);
	if (is_in_interactive_mode())
		rl_clear_history();
	free_data(d);
}

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
		result = check_and_run_bi(&d->envp, &d->envp_size, t->cmd_d, 1);
		free_for_bi_in_fork(d);
		exit (result);
	}
	close_ds_in_parent(t, ds, is_piped);
}
