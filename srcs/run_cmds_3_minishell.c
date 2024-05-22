/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_cmds_3_minishell.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 17:12:13 by hauerbac          #+#    #+#             */
/*   Updated: 2024/05/21 16:45:30 by hauerbac         ###   ########.fr       */
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
