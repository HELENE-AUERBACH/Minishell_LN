/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   files_management_3_minishell.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 14:56:52 by hauerbac          #+#    #+#             */
/*   Updated: 2024/05/27 15:03:03 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	open_out_file_and_close_prev(t_list **new_files,
			t_list **cmd_new_files, t_token *t, t_dll_el *prev)
{
	char	*file_name;

	if (!(t && new_files && cmd_new_files))
		return (-1);
	if (prev && close_previous_file(prev) < 0)
		return (-2);
	file_name = NULL;
	if (t->type == S_OUT_REDI || t->type == D_OUT_REDI)
	{
		file_name = (char *) malloc((t->src_len + 1) * sizeof(char));
		if (!file_name)
			return (-5);
		ft_strlcpy(file_name, t->src, t->src_len + 1);
		if (open_file(new_files, cmd_new_files, t, file_name) < 0)
			return (-6);
	}
	file_name = NULL;
	return (0);
}

int	check_files_for_out_redirections(t_dll_el **current,
		t_list **new_files, t_list **cmd_new_files,
		int redir_error_first_position)
{
	t_token		*t;
	int			result;

	t = cast_dll_el_into_token(current);
	while (t && (t->type == S_OUT_REDI || t->type == D_OUT_REDI)
		&& (redir_error_first_position == 0
			|| t->position < redir_error_first_position))
	{
		result = remove_first_spaces_and_ext_quotes(t);
		if (result != 0)
			return (result);
		if (check_out_redir_file(t) < 0)
			return (-1 * t->position);
		if (open_out_file_and_close_prev(new_files, cmd_new_files, t, \
			(*current)->prev) < 0)
			return (-1 * t->position);
		*current = (*current)->next;
		if (*current)
			t = (t_token *)((*current)->content);
		else
			t = NULL;
	}
	return (0);
}
