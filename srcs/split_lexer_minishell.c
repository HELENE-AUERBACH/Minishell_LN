/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_lexer_minishell.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 15:06:54 by hauerbac          #+#    #+#             */
/*   Updated: 2024/05/01 18:43:51 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_minishell.h"

static int	suppress_spaces(int i, int *data, const char *str)
{
	int	j;

	j = i;
	while (is_a_space(str[j])
		&& data[EXT_OPEN_S_QUOTE] == 0
		&& data[EXT_OPEN_D_QUOTE] == 0
		&& (str[j + 1] == '\0' || (str[j + 1] && str[j + 1] != '"'
				&& str[j + 1] != '\'' && str[j + 1] != '(')))
	{
		data[END]++;
		j++;
	}
	return (j);
}

static void	init_tokenizer_data(t_tokenizer_data *d, const char *str,
			t_dll *lst)
{
	d->data[STR_LEN] = ft_strlen(str);
	d->data[START] = 0;
	d->data[END] = 0;
	d->data[EXT_OPEN_D_QUOTE] = 0;
	d->data[EXT_OPEN_S_QUOTE] = 0;
	d->data[EXT_OPENING_PARENTHESIS] = 0;
	d->data[EXT_CLOSING_PARENTHESIS] = 0;
	d->i = 0;
	d->j = 0;
	d->position = 0;
	d->raw_command = NULL;
	d->lst = lst;
	d->dll_current_el_ptr = NULL;
}

int	split_in_tokens(const char *str, t_dll *lst)
{
	t_tokenizer_data	d;

	init_tokenizer_data(&d, str, lst);
	while (d.i < d.data[STR_LEN])
	{
		d.i = suppress_spaces(d.i, d.data, str);
		if (d.data[STR_LEN] - d.i <= 0)
			break ;
		if (has_wrong_start(str[d.i]))
			return (2);
		d.data[START] = d.i;
		d.data[END] = d.i;
		d.raw_command = (char *) malloc((d.data[STR_LEN] - d.i + 1)
				* sizeof(char));
		if (!d.raw_command)
			return (3);
		d.j = 0;
		ft_bzero(d.raw_command, d.data[STR_LEN] - d.i + 1);
		if (extract_tokens_into_dll(&d, str) < 0
			|| has_wrong_format(d.data, str))
			return (free(d.raw_command), 2);
		free(d.raw_command);
		d.raw_command = NULL;
	}
	return (0);
}

static void	check_builtin_type(int *type, int i, t_tokenizer_data *d)
{
	if (ft_strncmp("echo ", d->raw_command + i, 5) == 0
		|| (ft_strncmp("echo", d->raw_command + i, 4) == 0
			&& d->data[END] - d->data[START] + 1 == 4)
		|| ft_strncmp("cd ", d->raw_command + i, 3) == 0
		|| (ft_strncmp("cd", d->raw_command + i, 2) == 0
			&& d->data[END] - d->data[START] + 1 == 2)
		|| ft_strncmp("pwd ", d->raw_command + i, 4) == 0
		|| (ft_strncmp("pwd", d->raw_command + i, 3) == 0
			&& d->data[END] - d->data[START] + 1 == 3)
		|| ft_strncmp("export ", d->raw_command + i, 7) == 0
		|| (ft_strncmp("export", d->raw_command + i, 6) == 0
			&& d->data[END] - d->data[START] + 1 == 6)
		|| ft_strncmp("unset ", d->raw_command + i, 6) == 0
		|| (ft_strncmp("unset", d->raw_command + i, 5) == 0
			&& d->data[END] - d->data[START] + 1 == 5)
		|| ft_strncmp("env ", d->raw_command + i, 4) == 0
		|| (ft_strncmp("env", d->raw_command + i, 3) == 0
			&& d->data[END] - d->data[START] + 1 == 3)
		|| ft_strncmp("exit ", d->raw_command + i, 5) == 0
		|| (ft_strncmp("exit", d->raw_command + i, 4) == 0
			&& d->data[END] - d->data[START] + 1 == 4))
		*type = BI;
}

int	end_raw_command(t_tokenizer_data *d)
{
	int	type;
	int	i;
	int	result;

	result = 0;
	if (d && d->j > 0 && d->raw_command)
	{
		if (d->raw_command[d->j] && is_a_space(d->raw_command[d->j]))
		{
			d->data[END]--;
			d->j--;
		}
		d->raw_command[d->j] = '\0';
		type = COMMAND;
		i = 0;
		if (d->data[EXT_OPENING_PARENTHESIS] == 1)
			i = 1;
		check_builtin_type(&type, i, d);
		result = init_token(type, d->raw_command, d);
		d->j = 0;
		d->raw_command = NULL;
	}
	return (result);
}
