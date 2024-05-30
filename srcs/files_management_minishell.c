/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   files_management_minishell.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 20:53:05 by hauerbac          #+#    #+#             */
/*   Updated: 2024/05/28 15:54:34 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	save_created_file_name(t_list **new_files,
			t_list **cmd_new_files, char *file_name)
{
	t_list	*lst_new;
	t_list	*lst_new_2;

	if (access(file_name, F_OK) == -1)
	{
		lst_new = ft_lstnew((void *) file_name);
		if (!lst_new)
			return (3);
		ft_lstadd_back(new_files, lst_new);
		lst_new_2 = ft_lstnew((void *) file_name);
		if (!lst_new_2)
			return (3);
		ft_lstadd_back(cmd_new_files, lst_new_2);
	}
	return (0);
}

int	open_file(t_list **new_files, t_list **cmd_new_files, t_token *t,
		char *file_name)
{
	if (t->type == S_IN_REDI)
		t->cmd_d->file1 = file_name;
	if (t->type == S_IN_REDI || t->type == HEREDOC)
	{
		t->cmd_d->fd1 = open(t->cmd_d->file1, O_RDONLY);
		if (t->type == S_IN_REDI && t->cmd_d->fd1 == -1)
			return (display_err_with_prefix(t->cmd_d->file1, \
					" input file open error\n"), -1);
		else if (t->type == HEREDOC && t->cmd_d->fd1 == -1)
			return (display_error("T. input file open err.\n"), -1);
	}
	if (t->type == S_OUT_REDI || t->type == D_OUT_REDI)
	{
		t->cmd_d->file2 = file_name;
		if (save_created_file_name(new_files, cmd_new_files, \
			file_name) != 0)
			return (-2);
	}
	if (t->type == S_OUT_REDI)
		t->cmd_d->fd2 = open(file_name, O_WRONLY | O_CREAT | O_TRUNC,
				0644);
	if (t->type == D_OUT_REDI)
		t->cmd_d->fd2 = open(file_name, O_WRONLY | O_CREAT | O_APPEND,
				0644);
	return (0);
}

static int	open_in_file_and_close_prev(t_list **new_files,
			t_list **cmd_new_files, t_data *d, t_dll_el *current)
{
	char		*file_name;
	t_token		*t;

	t = cast_dll_el_into_token(&current);
	if (!(t && new_files && cmd_new_files))
		return (-1);
	if (current && current->prev && close_previous_file(current->prev) < 0)
		return (-2);
	if (t->type == HEREDOC && create_heredoc_file(d, t, current) < 0)
		return (-4);
	file_name = NULL;
	if (t->type == S_IN_REDI)
	{
		file_name = (char *) malloc((t->src_len + 1) * sizeof(char));
		if (!file_name)
			return (-5);
		ft_strlcpy(file_name, t->src, t->src_len + 1);
	}
	if (t->type == HEREDOC)
		file_name = t->cmd_d->file1;
	if (open_file(new_files, cmd_new_files, t, file_name) < 0)
		return (-6);
	if (t->type == HEREDOC)
		return (g_exit_status);
	return (0);
}

int	check_files_for_in_redirections(t_dll_el **current,
		int redir_error_first_position, t_data *d, int is_in_piped)
{
	t_token		*t;
	int			result;

	t = cast_dll_el_into_token(current);
	while (t && (t->type == S_IN_REDI || t->type == HEREDOC)
		&& (redir_error_first_position == 0
			|| t->position < redir_error_first_position))
	{
		result = remove_first_spaces_and_ext_quotes(t);
		if (result != 0)
			return (result);
		if (t->type == S_IN_REDI
			&& (check_in_redir_file(t, d->new_files, \
				d->cmd_new_files, is_in_piped) < 0))
			return (-1 * t->position);
		if (open_in_file_and_close_prev(&d->new_files, \
			&d->cmd_new_files, d, *current) < 0)
			return (-1 * t->position);
		*current = (*current)->next;
		if (*current)
			t = (t_token *)((*current)->content);
		else
			t = NULL;
	}
	return (0);
}
