/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_lexer_minishell.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 12:35:43 by hauerbac          #+#    #+#             */
/*   Updated: 2024/04/15 16:32:01 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_minishell.h"

static int	find_boundaries_of_redirection(int *type, int *j,
			t_tokenizer_data *d, const char *str)
{
	if (str[*j] == '<')
		*type = S_IN_REDI;
	if (str[*j] == '>')
		*type = S_OUT_REDI;
	if (str[d->i] == '<' || str[d->i] == '>')
	{
		if ((str[*j] == '>' && str[d->i] == '<')
			|| (str[*j] == '<' && is_a_space(str[d->i - 1])
				&& str[d->i] == '>'))
			return (-2);
		*type = S_OUT_REDI;
		if (str[d->i - 1] == '<' && str[d->i] == '<')
			*type = HEREDOC;
		if (str[d->i - 1] == '>' && str[d->i] == '>')
			*type = D_OUT_REDI;
		d->i++;
		while (is_a_space(str[d->i]))
			d->i++;
		if (has_wrong_start(str[d->i]))
			return (-2);
	}
	*j = get_end_index_of_file_or_delimiter_name(&d->i, str);
	return (0);
}

static int	extract_redirection(t_tokenizer_data *d, const char *str)
{
	int		j;
	int		type;
	char	*f_or_delim_name;

	if (d->i >= 0 && (str[d->i] == '<' || str[d->i] == '>'))
	{
		j = d->i;
		d->i++;
		while (is_a_space(str[d->i]))
			d->i++;
		if (has_wrong_start(str[d->i]))
			return (-2);
		type = 0;
		if (find_boundaries_of_redirection(&type, &j, d, str) < 0)
			return (-2);
		f_or_delim_name = (char *)malloc((j - d->i + 1) * sizeof(char));
		if (!f_or_delim_name)
			return (-3);
		ft_strlcpy(f_or_delim_name, str + d->i, j - d->i + 1);
		d->i = j;
		while (is_a_space(str[d->i]))
			d->i++;
		return (init_token(type, f_or_delim_name, d));
	}
	return (-1);
}

static int	extract_control_operator(t_tokenizer_data *d, const char *str)
{
	char	*control_operator;
	int		type;

	if (d->i >= 0 && (str[d->i] == '|' || str[d->i] == '&' \
				|| str[d->i] == ';'))
	{
		if (str[d->i] == '&')
			return (-2);
		if (d->i + 1 >= d->data[STR_LEN] || str[d->i + 1] == '|'
			|| str[d->i + 1] == '&' || str[d->i + 1] == ';')
			return (-2);
		control_operator = (char *) malloc(2 * sizeof(char));
		if (!control_operator)
			return (-3);
		control_operator[0] = str[d->i];
		d->i++;
		control_operator[1] = '\0';
		if (control_operator[0] == '|')
			type = PIPE;
		if (control_operator[0] == ';')
			type = SEMICOLON;
		d->dll_current_el_ptr = NULL;
		return (init_token(type, control_operator, d));
	}
	return (-1);
}

static void	extract_char_into_raw_command(t_tokenizer_data *d,
			const char *str)
{
	if (d->i >= 0 && is_a_char_of_raw_command(d->i, d->data, str))
	{
		d->raw_command[d->j] = str[d->i];
		d->data[END] = d->i;
		d->j++;
	}
	check_simple_quotes(d->i, d->data, str);
	check_double_quotes(d->i, d->data, str);
	check_parenthesis(d->i, d->data, str);
	d->i++;
	if (is_a_redirection(d->i, d->data, str))
	{
		d->raw_command[d->j] = ' ';
		d->j++;
	}
}

int	extract_tokens_into_dll(t_tokenizer_data *d, const char *str)
{
	if (d && d->lst && !d->dll_current_el_ptr)
		d->dll_current_el_ptr = d->lst->tail;
	while (str[d->i] && !(d->data[EXT_OPEN_S_QUOTE] == 0
			&& d->data[EXT_OPEN_D_QUOTE] == 0
			&& is_a_command_separator(str[d->i])))
	{
		if (is_a_redirection(d->i, d->data, str))
		{
			while (d->j > 1 && is_a_space(d->raw_command[d->j - 2])
				&& is_a_space(d->raw_command[d->j - 1]))
				d->j--;
			if (extract_redirection(d, str) < 0)
				return (-2);
		}
		else
			extract_char_into_raw_command(d, str);
	}
	if (end_raw_command(d) < 0)
		return (-2);
	if (d->data[EXT_OPEN_S_QUOTE] == 0
		&& d->data[EXT_OPEN_D_QUOTE] == 0
		&& str[d->i] && is_a_command_separator(str[d->i])
		&& extract_control_operator(d, str) < 0)
		return (-2);
	return (0);
}
