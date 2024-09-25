/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   perform_expansions_lexer_minishell.c               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmorice <rmorice@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 11:15:24 by hauerbac          #+#    #+#             */
/*   Updated: 2024/09/25 10:45:45 by rmorice          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_minishell.h"

/* ************************************************************************** */
/*                              is_a_special_char                             */
/* -------------------------------------------------------------------------- */
/* This function checks if the char receive as input is a "special char".     */
/* a "special char" is a single or a double quote, percent, dollar, opening   */
/* or closing bracket, tilde, grave, ampersand, question mark, semicolon,     */
/* opening or closing parenthesis, opening or closing square bracket,         */
/* asterisk, plus, minus, '@', colon, dot, comma, slash, equal, pipe or       */
/* new line)                                                                  */
/* Input :                                                                    */
/*  - const char c : the char to check                                        */
/* Return :                                                                   */
/*  - 1 : if the char given in input is a "special char"                      */
/*  - 0 : othewrwise                                                          */
/* ************************************************************************** */
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

/* ************************************************************************** */
/*                              init_utils_data                               */
/* -------------------------------------------------------------------------- */
/* This function initialises the utils_data structure received in arguments   */
/* This structure contained datas about the command line overall and actual   */
/* state                                                                      */
/* Input :                                                                    */
/*  - t_data *d : struct of datas about internal criteria of token to init    */
/* Return :                                                                   */
/*  - 0 : if everything goes well                                             */
/*  - 1 othewrwise                                                            */
/* ************************************************************************** */
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

/* ************************************************************************** */
/*                             replace_token_src                              */
/* -------------------------------------------------------------------------- */
/* This function replaces t->src with new_src (if it exist, otherwise it is   */
/* replace by a void string).                                                 */
/* The length of the src is then updates with the length of new_src           */
/* Inputs :                                                                   */
/*  - t_token *t : a structure that contained datas about the current token   */
/*  - char *new_src : the sub-string that defines the token                   */
/* Return :                                                                   */
/*  - 0 : if everything goes well                                             */
/*  - 3 : othewrwise (malloc error)                                           */
/* ************************************************************************** */
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

/* ************************************************************************** */
/*                           perform_one_expansion                            */
/* -------------------------------------------------------------------------- */
/* This function expands   */
/* Inputs :                                                                   */
/*  - char **new_src : a pointer to the sub-string that defines the token     */
/*  - int *d : array of datas about internal criteria of token                */
/*  - t_token *t : a structure that contained datas about the current token   */
/*  - void *param : void variable that will be convert into a struct t_data   */
/* Return :                                                                   */
/*  - 0 : if everything goes well                                             */
/*  - int : the error code of the problem encounter                           */
/* ************************************************************************** */
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

/* ************************************************************************** */
/*                             perform_expansions                             */
/* -------------------------------------------------------------------------- */
/* This function expands   */
/* Inputs :                                                                   */
/*  - t_dll_el *el_ptr :  */
/*  - void *param : void variable that will be convert into a struct t_data   */
/* Return :                                                                   */
/*  - 0 : if everything goes well                                             */
/*  - int : the error code of the problem encounter                           */
/* ************************************************************************** */
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
		init_utils_data_and_new_src(d, &new_src, t->src_len);
		while (d[END] < t->src_len)
		{
			result = loop_perform_expansions(&new_src, d, t, param);
			if (result != 0)
				return (result);
			d[END]++;
		}
		if (d[HAS_EXPANSIONS] == 1)
			replace_token_src(t, new_src);
		else if (d[HAS_EXPANSIONS] != 1 && new_src)
			free(new_src);
	}
	return (0);
}
