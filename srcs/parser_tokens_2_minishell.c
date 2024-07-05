/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_tokens_2_minishell.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 14:07:08 by hauerbac          #+#    #+#             */
/*   Updated: 2024/07/05 11:52:50 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_substr_after_spaces(t_token *t, int *utils_d)
{
	int		i;
	char	*s;

	i = 0;
	while (is_a_space(t->src[i]))
		i++;
	if (i > 0)
	{
		utils_d[START] = i;
		s = ft_substr(t->src, utils_d[START],
				t->src_len - utils_d[START]);
		if (!s)
			return (3);
		free(t->src);
		t->src = s;
		t->src_len = ft_strlen(s);
		utils_d[STR_LEN] = t->src_len;
		s = NULL;
	}
	return (0);
}

int	remove_first_spaces_and_ext_quotes(t_token *t)
{
	int		utils_d[9];
	char	*s;

	init_utils_data(utils_d, t->src_len);
	if (get_substr_after_spaces(t, utils_d) == 3)
		return (3);
	check_simple_quotes(0, utils_d, t->src);
	check_double_quotes(0, utils_d, t->src);
	if (utils_d[EXT_OPEN_S_QUOTE] == 1
		|| utils_d[EXT_OPEN_D_QUOTE] == 1)
	{
		utils_d[START] = 1;
		s = ft_substr(t->src, 1, t->src_len - 2);
		if (!s)
			return (3);
		free(t->src);
		t->src = s;
		t->src_len = ft_strlen(s);
		utils_d[STR_LEN] = t->src_len;
		s = NULL;
	}
	if ((t->type == COMMAND || t->type == BI) && t->src_len > 1
		&& has_wrong_start(t->src[0], '\0'))
		return (2);
	return (0);
}

int	is_a_pipe_or_ctrloperator(enum e_token_types type)
{
	if (type == PIPE || type == SEMICOLON || type == AMPERSAND
		|| type == DOUBLE_SEMICOLON || type == LOGICAL_AND
		|| type == LOGICAL_OR || type == SEMICOLON_AMPERSAND
		|| type == DOUBLE_SEMICOLON_AMPERSAND || type == PIPE_AMPERSAND)
		return (1);
	return (0);
}

static int	ft_min(int value1, int value2)
{
	if (value1 <= value2)
		return (value1);
	else
		return (value2);
}

int	check_redir_files(t_dll_el **current, t_data *d, int is_in_piped)
{
	int			redir_err;
	int			redir_err1;
	t_token		*t;

	if (!(current && *current && d))
		return (1);
	redir_err = 0;
	while (current && *current && (is_an_out_redirect((*current)->content) \
		|| is_an_in_redirect((*current)->content)))
	{
		redir_err1 = ft_min(check_files_for_in_redirections(current, \
				-1 * redir_err, d, is_in_piped), redir_err);
		redir_err = ft_min(check_files_for_out_redirections(current, \
		&d->new_files, &d->cmd_new_files, -1 * redir_err1), redir_err1);
		t = cast_dll_el_into_token(current);
		if (t && ((redir_err1 < 0 && t->position >= -1 * redir_err1) \
			|| (redir_err < 0 && t->position >= -1 * redir_err)))
			break ;
	}
	if (current && *current && (*current)->prev
		&& (is_an_out_redirect((*current)->prev->content) == 1
			|| is_an_in_redirect((*current)->prev->content) == 1))
		close_previous_file((*current)->prev);
	return (redir_err);
}
