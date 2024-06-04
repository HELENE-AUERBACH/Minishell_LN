/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   perform_expansions_2_lexer_minishell.c             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 12:56:53 by hauerbac          #+#    #+#             */
/*   Updated: 2024/06/04 15:24:38 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_not_a_valid_identifier(t_token *t, int i, int j, char **new_src)
{
	if (i > 0 && i < t->src_len && t->src[i] && t->src[i - 1] == '{' \
		&& ((t->src[i] != '?' && is_a_special_char(t->src[i])) \
		|| (j < t->src_len && j > i \
		&& (t->src[j] != '}' || ((j - 1 > i || t->src[i] != '?') \
		&& is_a_special_char(t->src[j - 1])))) \
		|| (!ends_with_a_closing_brace(t, i))))
	{
		if (*new_src)
			free(*new_src);
		while (i > 0 && t->src[i] != '$')
			i--;
		j--;
		if (t->src[i + 1] == '{')
		{
			while (j < t->src_len && t->src[j] && t->src[j] != '}')
				j++;
		}
		*new_src = ft_substr(t->src, i, j - i + 1);
		if (!*new_src)
			return (display_error("bad substitution\n"), 1);
		return (display_err_with_prefix(*new_src, \
				"bad substitution\n"), free(*new_src), 1);
	}
	return (0);
}

static int	get_new_src_before_expansion(char **new_src, int *d, t_token *t)
{
	char	*s;

	while (d[END] < t->src_len && t->src[d[END]] == '$'
		&& d[END] + 1 < t->src_len && t->src[d[END] + 1] == '$')
		d[END]++;
	if (d[END] < t->src_len && t->src[d[END]] == '$'
		&& (d[END] + 1 == t->src_len || (d[END] + 1 < t->src_len \
			&& is_a_space(t->src[d[END] + 1]))))
		d[END]++;
	if (!*new_src && d[START] != d[END])
	{
		*new_src = ft_substr(t->src, d[START], d[END] - d[START]);
		if (!*new_src)
			return (3);
	}
	else if (*new_src && d[START] != d[END])
	{
		s = ft_substr(t->src, d[START], d[END] - d[START]);
		if (!s)
			return (3);
		*new_src = ft_strjoin_with_free(*new_src, s);
		if (!*new_src)
			return (3);
	}
	return (0);
}

int	get_new_src_for_expansion(char **new_src, t_token *t, int *d,
		void *param)
{
	char	*name;
	char	*value;
	t_data	*minishell_data;
	int		result;

	minishell_data = (t_data *) param;
	name = ft_substr(t->src, d[I], d[J] - 1 - d[I] + 1);
	if (!name)
		return (3);
	value = NULL;
	result = get_value_from_minishell_envp(&value, name, minishell_data);
	free(name);
	if (result != 0)
		return (result);
	if (value && !*new_src)
		*new_src = value;
	else if (value)
	{
		*new_src = ft_strjoin_with_free(*new_src, value);
		if (!*new_src)
			return (3);
	}
	return (0);
}

static int	get_new_src_after_expansion(char **new_src, t_token *t, int *d)
{
	char	*s;

	if (!(*new_src && d[START] < t->src_len && t->src[d[START]]))
		return (0);
	while (d[END] + 1 < t->src_len && t->src[d[END] + 1]
		&& (t->src[d[END] + 1] != '$' || d[EXT_OPEN_S_QUOTE] == 1 \
		|| (t->src[d[END] + 1] == '$' && d[EXT_OPEN_S_QUOTE] == 0 \
		&& d[END] + 2 < t->src_len && (is_a_space(t->src[d[END] + 2]) \
		|| t->src[d[END] + 2] == '$'))))
	{
		d[END]++;
		check_simple_quotes(d[END], d, t->src);
		check_double_quotes(d[END], d, t->src);
	}
	if (d[END] - d[START] + 1 == 0)
		return (0);
	s = ft_substr(t->src, d[START], d[END] - d[START] + 1);
	if (!s)
		return (3);
	d[START] = d[END] + 1;
	*new_src = ft_strjoin_with_free(*new_src, s);
	if (!*new_src)
		return (3);
	return (0);
}

int	loop_perform_expansions(char **new_src, int *d, t_token *t, void *param)
{
	int	result;

	result = 0;
	check_simple_quotes(d[END], d, t->src);
	check_double_quotes(d[END], d, t->src);
	if (d[END] < t->src_len && t->src && t->src[d[END]]
		&& d[EXT_OPEN_S_QUOTE] == 0 && t->src[d[END]] == '$'
		&& d[END] + 1 < t->src_len && !is_a_space(t->src[d[END] + 1]))
	{
		result = get_new_src_before_expansion(new_src, d, t);
		if (result != 0)
			return (result);
		if (d[END] < t->src_len && t->src[d[END]]
			&& t->src[d[END]] == '$' && d[END] + 1 < t->src_len
			&& !is_a_space(t->src[d[END] + 1]))
		{
			d[HAS_EXPANSIONS] = 1;
			result = perform_one_expansion(new_src, d, t, param);
			if (result != 0)
				return (result);
		}
	}
	if (d[HAS_EXPANSIONS] == 0)
		return (0);
	return (get_new_src_after_expansion(new_src, t, d));
}
