/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_lexer_minishell.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmorice <rmorice@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 15:06:54 by hauerbac          #+#    #+#             */
/*   Updated: 2024/08/20 11:46:16 by rmorice          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_minishell.h"

/* ************************************************************************** */
/*                              suppress_spaces                               */
/* -------------------------------------------------------------------------- */
/* This function moves to the first that isn't a space (if the space is into  */
/* quotes then the space isn't suppress)                                      */
/* Inputs :                                                                   */
/*  - int i : the index of the first char to look at                          */
/*  - int *data : array that contained infos about the current situation      */
/*  - const char *str : the line that we are processing                       */
/* Return :                                                                   */
/*  - int  : the index of the first index that isn't a space (out of quotes)  */
/* ************************************************************************** */
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

/* ************************************************************************** */
/*                            init_tokenizer_data                             */
/* -------------------------------------------------------------------------- */
/* This function initialses the tokenizer data that contained informations    */
/* relative to the current state of the command analyse                       */
/* Inputs :                                                                   */
/*  - t_tokenizer_data *d : a pointer to the structure to initialise          */
/*  - const char *str : the line that we are processing                       */
/*  - t_dll *lst : the double list       */
/* Return :                                                                   */
/*  - None                                                                    */
/* ************************************************************************** */
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

/* ************************************************************************** */
/*                              split_in_tokens                               */
/* -------------------------------------------------------------------------- */
/* This function splits the string into tokens                                */
/* Inputs :                                                                   */
/*  - const char *str : the string to split                                   */
/*  - t_dll *lst : the double list       */
/* Return :                                                                   */
/*  - 0 : if everything goes well                                             */
/*  - int : the error code of the problem encounter                           */
/* ************************************************************************** */
int	split_in_tokens(const char *str, t_dll *lst)
{
	t_tokenizer_data	d;

	init_tokenizer_data(&d, str, lst);
	while (d.i < d.data[STR_LEN])
	{
		d.i = suppress_spaces(d.i, d.data, str);
		if (d.data[STR_LEN] - d.i <= 0)
			break ;
		if (has_wrong_start(str[d.i], '\0'))
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

/* ************************************************************************** */
/*                             check_builtin_type                             */
/* -------------------------------------------------------------------------- */
/* This function changes the type to "BI" (6) if the raw command corresponds  */
/* to a builtin function (echo, cd, pwd, export, unset, env, exit)            */
/* Inputs :                                                                   */
/*  - int *type : a pointer to an int that define the kind of command met     */
/*  - int i : the offset to ignore, if needed, an open bracket                */
/*  - t_tokenizer_data *d : a struct about the current state of cmd analyse   */
/* Return :                                                                   */
/*  - 0 : if everything goes well                                             */
/*  - int : the error code of the problem encounter                           */
/* ************************************************************************** */
static void	check_builtin_type(int *type, int i, t_tokenizer_data *d)
{
	if ((ft_strncmp("echo", d->raw_command + i, 4) == 0
			&& (d->raw_command[i + 4] == '\0'
				|| is_a_space(d->raw_command[i + 4])))
		|| (ft_strncmp("cd", d->raw_command + i, 2) == 0
			&& (d->raw_command[i + 2] == '\0'
				|| is_a_space(d->raw_command[i + 2])))
		|| (ft_strncmp("pwd", d->raw_command + i, 3) == 0
			&& (d->raw_command[i + 3] == '\0'
				|| is_a_space(d->raw_command[i + 3])))
		|| (ft_strncmp("export", d->raw_command + i, 6) == 0
			&& (d->raw_command[i + 6] == '\0'
				|| is_a_space(d->raw_command[i + 6])))
		|| (ft_strncmp("unset", d->raw_command + i, 5) == 0
			&& (d->raw_command[i + 5] == '\0'
				|| is_a_space(d->raw_command[i + 5])))
		|| (ft_strncmp("env", d->raw_command + i, 3) == 0
			&& (d->raw_command[i + 3] == '\0'
				|| is_a_space(d->raw_command[i + 3])))
		|| (ft_strncmp("exit", d->raw_command + i, 4) == 0
			&& (d->raw_command[i + 4] == '\0'
				|| is_a_space(d->raw_command[i + 4]))))
		*type = BI;
}

/* ************************************************************************** */
/*                              end_raw_command                               */
/* -------------------------------------------------------------------------- */
/* This function decrementes the index of the ending of the command if the    */
/* raw command ended with a space. It also specified if the command type is   */
/* "BI" or "COMMAND" and initialised the datas relative to this token         */
/* Input :                                                                    */
/*  - t_tokenizer_data *d : a struct about the current state of cmd analyse   */
/* Return :                                                                   */
/*  - 0 : if everything goes well                                             */
/*  - int : otherwise                                                         */
/* ************************************************************************** */
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
