/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_lexer_minishell.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmorice <rmorice@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 12:35:43 by hauerbac          #+#    #+#             */
/*   Updated: 2024/09/16 17:18:03 by rmorice          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_minishell.h"

/* ************************************************************************** */
/*                       find_boundaries_of_redirection                       */
/* -------------------------------------------------------------------------- */
/* This function determines if the redirection encounter is of type '<', '>'  */
/* '<<' or '>>' (Single in or out redirection, pseudo heredoc or double out)  */
/* It checks if it is well formulated and places the index j right after the  */
/* end of file name or delimiter word                                         */
/* Inputs :                                                                   */
/*  - int *type : a pointer to an int that define the kind of redirection met */
/*  - int *j : a pointer to the index that should point to the end of name    */
/*  - t_tokenizer_data *d : a structure that contained analyse status datas   */
/*  - const char *str : the command line use as reference                     */
/* Return :                                                                   */
/*  - 0 : if everything goes well                                             */
/*  - -2 : otherwise                                                          */
/* ************************************************************************** */
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
		if (has_wrong_start(str[d->i], str[*j]))
			return (-2);
	}
	*j = get_end_index_of_file_or_delimiter_name(&d->i, str);
	return (0);
}

/* ************************************************************************** */
/*                            extract_redirection                             */
/* -------------------------------------------------------------------------- */
/* This function detemines the type of redirection that we have as well as    */
/* the file name or delimiter (pseudo-heredoc) associated.                    */
/* The index i is move until the next character that isn´t a space and the    */
/* structure d is reinitialized with informations relative to the token       */
/* Inputs :                                                                   */
/*  - t_tokenizer_data *d : a struct about current state of the cmd analyse   */
/*  - const char *str : the command line to use as reference                  */
/* Return :                                                                   */
/*  - 0 : if everything goes well                                             */
/*  - int : the error code of the problem encounter (negative values)         */
/* ************************************************************************** */
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
		if (has_wrong_start(str[d->i], str[j]))
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

/* ************************************************************************** */
/*                          extract_control_operator                          */
/* -------------------------------------------------------------------------- */
/* This function determines if the char that we are checking is a control     */
/* operator ('|', ';' or '&'). AS of now we don't take into account '&' but   */
/* if it is one of the other two operators then the type of the token is      */
/* adapt to specified if we have a PIPE or a SEMICOLON                        */
/* Inputs :                                                                   */
/*  - t_tokenizer_data *d : a struct about current state of the cmd analyse   */
/*  - const char *str : the command line to use as reference                  */
/* Return :                                                                   */
/*  - O : if everything goes well                                             */
/*  - int : otherwise                                                         */
/* ************************************************************************** */
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

/* ************************************************************************** */
/*                       extract_char_into_raw_command                        */
/* -------------------------------------------------------------------------- */
/* This function updates d->raw_command to make it point toward str[d->i] if  */
/* this char is considered as one of a raw command.                           */
/* We also update, if needed, datas about quotes (single and double) as well  */
/* as parenthesis                                                             */
/* ??? */
/* Inputs :                                                                   */
/*  - t_tokenizer_data *d : a struct about current state of the cmd analyse   */
/*  - const char *str : the command line to use as reference                  */
/* Return :                                                                   */
/*  - None                                                                    */
/* ************************************************************************** */
// why do we do i++ in every cases ??? (be it a char of raw_command, or a
// quote or a parenthesis or none of the previous...)
// and why do we check if we have a redirection right after the char that we check ???
// And why do we ignored this redirection in that case ???
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

/* ************************************************************************** */
/*                          extract_tokens_into_dll                           */
/* -------------------------------------------------------------------------- */
/* This function  */
/* Inputs :                                                                   */
/*  - t_tokenizer_data *d : a struct about current state of the cmd analyse   */
/*  - const char *str : the command line to use as reference                  */
/* Return :                                                                   */
/*  - 0 : if everything goes well                                             */
/*  - int : the error code of the problem encounter                           */
/* ************************************************************************** */
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
