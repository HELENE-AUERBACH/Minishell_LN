/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks_split_lexer_minishell.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 12:28:43 by hauerbac          #+#    #+#             */
/*   Updated: 2024/07/05 11:57:52 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_minishell.h"

void	check_double_quotes(int i, int *data, const char *str)
{
	if (str[i] == '"' && data[EXT_OPEN_S_QUOTE] == 0)
	{
		if (data[EXT_OPEN_D_QUOTE] == 0)
			data[EXT_OPEN_D_QUOTE] = 1;
		else if (data[EXT_OPEN_D_QUOTE] == 1)
			data[EXT_OPEN_D_QUOTE] = 0;
	}
}

void	check_simple_quotes(int i, int *data, const char *str)
{
	if (str[i] == '\'' && data[EXT_OPEN_D_QUOTE] == 0)
	{
		if (data[EXT_OPEN_S_QUOTE] == 0)
			data[EXT_OPEN_S_QUOTE] = 1;
		else if (data[EXT_OPEN_S_QUOTE] == 1)
			data[EXT_OPEN_S_QUOTE] = 0;
	}
}

void	check_parenthesis(int i, int *data, const char *str)
{
	if (str[i] == '(' && data[EXT_OPEN_S_QUOTE] == 0
		&& data[EXT_OPEN_D_QUOTE] == 0)
	{
		data[EXT_OPENING_PARENTHESIS]++;
	}
	if (str[i] == ')' && data[EXT_OPEN_S_QUOTE] == 0
		&& data[EXT_OPEN_D_QUOTE] == 0)
	{
		data[EXT_CLOSING_PARENTHESIS]++;
	}
}

int	has_wrong_format(int *data, const char *str)
{
	if (data[EXT_OPEN_S_QUOTE] == 1)
		return (display_error("Wrong number of external '\n"), 1);
	if (data[EXT_OPEN_D_QUOTE] == 1)
		return (display_error("Wrong number of external \"\n"), 1);
	if (data[EXT_OPENING_PARENTHESIS] != data[EXT_CLOSING_PARENTHESIS])
		return (display_error("Wrong number of parentheses\n"), 1);
	if (str[data[START]] == ')' || str[data[END]] == '(')
		return (display_error("Wrong format of parentheses\n"), 1);
	return (0);
}

int	has_wrong_start(const char c, const char c2)
{
	if (is_a_command_separator(c))
	{
		if (c != '\0')
			display_syntax_error(c);
		else
			display_syntax_error(c2);
		return (1);
	}
	return (0);
}
