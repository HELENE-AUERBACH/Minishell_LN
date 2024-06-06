/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_tokens_minishell.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <hauerbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 11:23:08 by hauerbac          #+#    #+#             */
/*   Updated: 2024/06/06 14:35:53 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	find_next_pipe_or_ctrloperator(t_token **t_cmdbi, int *is_piped,
			t_dll_el **current)
{
	t_token	*t;
	int		found;

	if (t_cmdbi && *t_cmdbi)
		(*t_cmdbi)->cmd_d->is_in_piped = *is_piped;
	found = 0;
	while (current && *current && (*current)->content && !found)
	{
		t = (t_token *)((*current)->content);
		if (t && is_a_pipe_or_ctrloperator(t->type))
		{
			found = 1;
			*is_piped = 0;
			if (t->type == PIPE)
				*is_piped = 1;
			if (t_cmdbi && *t_cmdbi)
				(*t_cmdbi)->cmd_d->followed_by_op = t->type;
		}
		*current = (*current)->next;
	}
	if (!found)
		*is_piped = 0;
	if (t_cmdbi && *t_cmdbi)
		(*t_cmdbi)->cmd_d->is_out_piped = *is_piped;
}

static void	ignore_others_redirections(t_dll_el **current)
{
	t_token	*t;

	if (current && *current)
	{
		t = (t_token *)((*current)->content);
		while (t && (t->type == S_IN_REDI || t->type == HEREDOC \
			|| t->type == S_OUT_REDI || t->type == D_OUT_REDI))
		{
			*current = (*current)->next;
			if (*current)
				t = (t_token *)((*current)->content);
			else
				t = NULL;
		}
	}
}

static void	handle_a_command_or_a_bi(t_token **t_cmdbi, t_dll_el **current,
			int result_of_check_files_for_redirections)
{
	t_token	*t;

	*t_cmdbi = NULL;
	if (current && *current && (*current)->content)
	{
		t = (t_token *)((*current)->content);
		if (t && (t->type == COMMAND || t->type == BI))
		{
			if (result_of_check_files_for_redirections == 0
				&& t && t->cmd_d)
			{
				*t_cmdbi = t;
				t->cmd_d->args = split_cmd_args(t);
				if (t->cmd_d->args && t->cmd_d->args[0])
					t->cmd_d->cmd = (char *) malloc(sizeof(\
						char) * (ft_strlen(\
						t->cmd_d->args[0]) + 1));
				if (t->cmd_d->args && t->cmd_d->args[0]
					&& t->cmd_d->cmd)
					t->cmd_d->cmd[0] = '\0';
			}
			*current = (*current)->next;
		}
	}
}

static int	find_cmd_or_bi(t_token **t_cmdbi, int *is_piped,
			t_dll_el **current, t_data *d)
{
	int			redir_error_first_position;
	int			result;
	t_dll_el	*start_el;

	result = 0;
	start_el = *current;
	redir_error_first_position = check_redir_files(current, d, *is_piped);
	result = redir_error_first_position;
	if (redir_error_first_position < 0)
	{
		ignore_others_redirections(current);
		result = 1;
	}
	handle_a_command_or_a_bi(t_cmdbi, current, result);
	if (t_cmdbi && *t_cmdbi && (*t_cmdbi)->cmd_d && (*t_cmdbi)->cmd_d->args
		&& (*t_cmdbi)->cmd_d->args[0] && !(*t_cmdbi)->cmd_d->cmd)
		result = 3;
	if (result == 0 && t_cmdbi && *t_cmdbi && (*t_cmdbi)->cmd_d
		&& (*t_cmdbi)->cmd_d->cmd)
		result = set_last_redir_files_names(*t_cmdbi, start_el);
	find_next_pipe_or_ctrloperator(t_cmdbi, is_piped, current);
	empty_list(&d->cmd_new_files);
	return (result);
}

int	parse_tokens(t_data *d)
{
	t_dll_el	*el_ptr;
	t_token		*t_cmdbi;
	int			result;
	int			is_piped;
	int			forced;

	if (!(d && d->lst && d->lst->size > 0))
		return (-2);
	is_piped = 0;
	el_ptr = d->lst->head;
	t_cmdbi = NULL;
	result = 0;
	forced = 0;
	while (el_ptr && !(result == 3 || result == -3))
	{
		result = find_cmd_or_bi(&t_cmdbi, &is_piped, &el_ptr, d);
		if (result == 0 && t_cmdbi)
		{
			result = add_to_cmds_list(d, t_cmdbi);
			force_in_redir_of_next_cmdbi(t_cmdbi, &forced, &result);
		}
		else if (!(result == 3 || result == -3))
			forced = 1;
	}
	return (result);
}
