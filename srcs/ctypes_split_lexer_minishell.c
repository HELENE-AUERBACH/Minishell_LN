/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ctypes_split_lexer_minishell.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 12:25:19 by hauerbac          #+#    #+#             */
/*   Updated: 2024/03/12 13:20:32 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_minishell.h"

int	is_a_space(const char c)
{
	if (c != '\0'
		&& (c == ' ' || c == '\t' || c == '\v' || c == '\f'
			|| c == '\r'))
		return (1);
	return (0);
}

int	is_a_metacharacter(const char c)
{
	if (c == '\0' || c == ' ' || c == '\n' || c == '|' || c == '&'
		|| c == ';' || c == '(' || c == ')' || c == '<' || c == '>'
		|| c == '\t' || c == '\v' || c == '\f' || c == '\r')
		return (1);
	return (0);
}

int	is_a_command_separator(const char c)
{
	if (c == '\0' || c == '\n' || c == '|' || c == '&' || c == ';')
		return (1);
	return (0);
}

int	is_a_redirection(int i, int *data, const char *str)
{
	if (data[EXT_OPEN_S_QUOTE] == 0
		&& data[EXT_OPEN_D_QUOTE] == 0
		&& (str[i] == '<' || str[i] == '>'))
		return (1);
	return (0);
}

int	is_a_char_of_raw_command(int i, int *data, const char *str)
{
	if (!is_a_metacharacter(str[i])
		|| str[i] == '(' || str[i] == ')'
		|| data[EXT_OPEN_S_QUOTE] == 1
		|| data[EXT_OPEN_D_QUOTE] == 1
		|| (i + 1 < data[STR_LEN] && is_a_space(str[i])
			&& !is_a_command_separator(str[i + 1])
			&& !is_a_space(str[i + 1])))
		return (1);
	return (0);
}
