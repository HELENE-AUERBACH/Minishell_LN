/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   files_management_2_minishell.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 20:53:05 by hauerbac          #+#    #+#             */
/*   Updated: 2024/05/16 13:47:38 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_minishell.h"

t_token	*cast_dll_el_into_token(t_dll_el **current)
{
	if (!(current && *current))
		return (NULL);
	return ((t_token *)((*current)->content));
}

int	close_previous_file(t_dll_el *prev)
{
	t_token	*p;

	if (!(prev && prev->content))
		return (0);
	p = (t_token *)(prev->content);
	if (p->cmd_d && p->cmd_d->fd1 != -1 && close(p->cmd_d->fd1) == -1)
	{
		if (p->cmd_d->is_here_doc == 0)
			display_err_with_prefix(p->cmd_d->file1, \
					" input file close error\n");
		else
			display_error("Temporary file close error\n");
		return (-1);
	}
	p->cmd_d->fd1 = -1;
	if (p->cmd_d && p->cmd_d->fd2 != -1 && close(p->cmd_d->fd2) == -1)
	{
		display_err_with_prefix(p->cmd_d->file2,
			" output file close error\n");
		return (-2);
	}
	p->cmd_d->fd2 = -1;
	return (0);
}

static int	is_file_name_in_list(char *file_name, int file_name_len,
			t_list *lst)
{
	t_list	*current;

	if (lst)
	{
		current = lst;
		while (current)
		{
			if (ft_strncmp(file_name, (char *) current->content, \
				file_name_len) == 0)
				return (1);
			current = current->next;
		}
	}
	return (0);
}

int	check_in_redir_file(t_token *t, t_list *new_files,
		t_list *cmd_new_files, int is_in_piped)
{
	if (!(t && t->type == S_IN_REDI))
		return (-1);
	if (access(t->src, F_OK) == -1 || access(t->src, R_OK) == -1)
		return (perror(t->src), -2);
	if (is_in_piped == 1 \
		&& is_file_name_in_list(t->src, t->src_len, new_files) == 1 \
		&& is_file_name_in_list(t->src, t->src_len, cmd_new_files) == 0)
		return (write(2, t->src, t->src_len), \
			write(2, ": No such file or directory\n", 28), -3);
	return (0);
}

int	check_out_redir_file(t_token *t)
{
	if (!(t && (t->type == S_OUT_REDI || t->type == D_OUT_REDI)))
		return (-1);
	if (access(t->src, F_OK) == 0 && access(t->src, W_OK) == -1)
		return (perror(t->src), -2);
	return (0);
}
