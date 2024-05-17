/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   perform_expansions_lexer_minishell.c               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 11:15:24 by hauerbac          #+#    #+#             */
/*   Updated: 2024/04/15 17:51:51 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_minishell.h"

int	is_a_special_char(const char c)
{
	if (is_a_space(c) || c == '\'' || c == '"' || c == '%' || c == '$'
		|| c == '{' || c == '}' || c == '~' || c == '`' || c == '&'
		|| c == '?' || c == ';' || c == '(' || c == ')' || c == '['
		|| c == ']' || c == '*' || c == '+' || c == '-' || c == '@'
		|| c == ':' || c == '.' || c == ',' || c == '/' || c == '='
		|| c == '|' || c == '\n')
		return (1);
	return (0);
}

void	init_utils_data(int *utils_data, int str_len)
{
	utils_data[STR_LEN] = str_len;
	utils_data[START] = 0;
	utils_data[END] = 0;
	utils_data[EXT_OPEN_D_QUOTE] = 0;
	utils_data[EXT_OPEN_S_QUOTE] = 0;
	utils_data[EXT_OPENING_PARENTHESIS] = 0;
	utils_data[EXT_CLOSING_PARENTHESIS] = 0;
	utils_data[I] = 0;
	utils_data[J] = 0;
	utils_data[HAS_EXPANSIONS] = 0;
}

static int	replace_token_src(t_token *t, char *new_src)
{
	if (t)
	{
		if (!new_src)
		{
			new_src = (char *) malloc(sizeof(char));
			if (!new_src)
				return (3);
			new_src[0] = '\0';
		}
		free(t->src);
		t->src = new_src;
		t->src_len = ft_strlen(new_src);
	}
	return (0);
}

int	perform_one_expansion(char **new_src, int *d, t_token *t, void *param)
{
	int	result;

	result = 0;
	if (d[END] < t->src_len && t->src && t->src[d[END]] == '$')
	{
		d[I] = d[END] + 1;
		if (d[I] < t->src_len && t->src[d[I]] == '{')
			d[I]++;
		d[J] = d[I] + 1;
		while (d[J] < t->src_len && (!is_a_special_char(t->src[d[J]]) \
			|| (t->src[d[I]] == '{' && t->src[d[J]] == '}' \
				&& d[J] - 1 >= 0 && t->src[d[J] - 1] != '}')))
			d[J]++;
		result = is_not_a_valid_identifier(t, d[I], d[J], new_src);
		if (result != 0)
			return (result);
		d[END] = d[J] - 1;
		if (d[J] >= 0 && d[J] < t->src_len && t->src[d[J]] == '}')
			d[END] = d[J];
		result = get_new_src_for_expansion(new_src, t, d, param);
		if (result != 0)
			return (result);
		d[START] = d[END] + 1;
	}
	return (result);
}

int	perform_expansions(t_dll_el *el_ptr, void *param)
{
	t_token		*t;
	int			d[10];
	char		*new_src;
	int			result;

	if (!(el_ptr && el_ptr->content && param))
		return (-2);
	t = (t_token *) el_ptr->content;
	if (t->type == S_IN_REDI || t->type == HEREDOC || t->type == S_OUT_REDI
		|| t->type == COMMAND || t->type == D_OUT_REDI || t->type == BI)
	{
		init_utils_data(d, t->src_len);
		new_src = NULL;
		while (d[END] < t->src_len)
		{
			result = loop_perform_expansions(&new_src, d, t, param);
			if (result != 0)
				return (result);
			d[END]++;
		}
		if (d[HAS_EXPANSIONS] == 1)
			replace_token_src(t, new_src);
	}
	return (0);
}
