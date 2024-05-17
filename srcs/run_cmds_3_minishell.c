/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_cmds_3_minishell.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 17:12:13 by hauerbac          #+#    #+#             */
/*   Updated: 2024/05/16 16:58:45 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	files_open(t_token *t)
{
	if (t && t->cmd_d && t->cmd_d->file1
		&& access(t->cmd_d->file1, F_OK) == 0
		&& access(t->cmd_d->file1, R_OK) == 0)
	{
		t->cmd_d->fd1 = open(t->cmd_d->file1, O_RDONLY);
		if (t->cmd_d->is_here_doc == 0 && t->cmd_d->fd1 == -1)
			return (display_err_with_prefix(t->cmd_d->file1, \
					" input file open error\n"), -1);
		if (t->cmd_d->is_here_doc == 1 && t->cmd_d->fd1 == -1)
			return (display_error("Temporary file open error\n"), \
					-1);
	}
	if (t && t->cmd_d && t->cmd_d->is_fd2_o_append == 0 && t->cmd_d->file2)
		t->cmd_d->fd2 = open(t->cmd_d->file2, \
					O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (t && t->cmd_d && t->cmd_d->is_fd2_o_append == 1 && t->cmd_d->file2)
		t->cmd_d->fd2 = open(t->cmd_d->file2, \
					O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (t && t->cmd_d && t->cmd_d->file2 && t->cmd_d->fd2 == -1)
		return (display_err_with_prefix(t->cmd_d->file2, \
				" output file open error\n"), -2);
	return (0);
}

void	close_files_on_err(int *result, t_cmd *cmd_d, const char *s,
		int new_result)
{
	if (cmd_d && cmd_d->fd1 != -1)
	{
		perror_and_code(result, s, new_result);
		close_in_file_and_free_file_name(cmd_d);
	}
	if (cmd_d && cmd_d->fd2 != -1)
	{
		perror_and_code(result, s, new_result);
		close_out_file_and_free_file_name(cmd_d);
	}
}

int	run_bi_without_fork(char ***envp, int *envp_size, t_cmd *cmd_d)
{
	int	result;

	result = 0;
	if (!(envp && *envp && envp_size && cmd_d))
		return (-1);
	if (cmd_d->fd1 != -1 && dup2(cmd_d->fd1, 0) == -1)
		close_files_on_err(&result, cmd_d, BI_REDIR_IN_ERR, -4);
	if (cmd_d->fd2 != -1 && dup2(cmd_d->fd2, 1) == -1)
		close_files_on_err(&result, cmd_d, BI_REDIR_OUT_ERR, -5);
	if (result == 0)
		result = check_builtin_type_and_run_bi(envp, envp_size, cmd_d);
	if (cmd_d->fd1 != -1)
		close_in_file_and_free_file_name(cmd_d);
	if (cmd_d->fd2 != -1)
		close_out_file_and_free_file_name(cmd_d);
	return (result);
}

void	run_bi_in_fork(t_data *d, t_token *t, int ds[3])
{
	const int	is_piped = (ds[0] != -1 && ds[1] != -1);
	int			result;

	t->cmd_d->pid = fork();
	if ((int) t->cmd_d->pid == -1)
		perr_cds(d, "Fork error", ds, is_piped);
	if ((int) t->cmd_d->pid == 0)
	{
		set_signals_actions_in_fork();
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
		result = check_builtin_type_and_run_bi(&d->envp, &d->envp_size,
				t->cmd_d);
		exit (result);
	}
	close_ds_in_parent(t, ds, is_piped);
}
