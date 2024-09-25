/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   perform_expansions_2_lexer_minishell.c             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmorice <rmorice@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 12:56:53 by hauerbac          #+#    #+#             */
/*   Updated: 2024/09/25 14:25:14 by rmorice          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* ************************************************************************** */
/*                         is_not_a_valid_identifier                          */
/* -------------------------------------------------------------------------- */
/* This function checks if is a "valid identifier" or not.                    */
/* If it isn't a "valid identifier" then an error message is display and the  */
/* function returns 1. Otherwise the return value will be 0                   */
/* rq : A "valid identifier" is either :                                      */
/*   - a question mark or a valid name (start with alphabetic char or an      */
/* underscore).                                                               */
/*   - a string that start with a '{' followed by a char that isn't a special */
/* char and end with a '}' preceded by a char that isn't a special char       */
/*   - a string that start with a '{' and end with a '}' between which we     */
/* don't have a question mark ({?})                                           */
/* Inputs :                                                                   */
/*  - t_token *t : a structure that contained datas about the current token   */
/*  - int i : index of the beggining of the possible identifier to check      */
/*  - int j : index of the ending of the possible identifier to check         */
/*  - char **new_src : a pointer to the sub-string that defines the token     */
/* Return :                                                                   */
/*  - 0 : if it is a "valid identifier"                                       */
/*  - 1 : othewrwise                                                          */
/* ************************************************************************** */
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

/* ************************************************************************** */
/*                        get_new_src_before_expansion                        */
/* -------------------------------------------------------------------------- */
/* This function looks for the last consecutive '$'. If if it is followed by  */
/* a space then the index of the end of the sub-string that will become the   */
/* token is incremented to keep the space.                                    */
/* If a sub-string already exist then we add the new substring obtained at    */
/* the end of it. Otherwise we create a new subtring.                         */
/* The sub-string will correspond to the string that precedes the expansion   */
/* If no expansion is expected then the substring will be the token as it is  */
/* Inputs :                                                                   */
/*  - char **new_src : a pointer to the sub-string that defines the token     */
/*  - int *d : array of datas about internal criteria of token                */
/*  - t_token *t : a structure that contained datas about the current token   */
/* Return :                                                                   */
/*  - 0 : if everything goes well                                             */
/*  - int : the error code of the problem encounter                           */
/* ************************************************************************** */
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

/* ************************************************************************** */
/*                         get_new_src_for_expansion                          */
/* -------------------------------------------------------------------------- */
/* This function replaces the part to expand by the associated value. To do   */
/* so a substring is created with the "name" of the variable to expand then   */
/* the current environment of our shell is check to find the value associated */
/* The expand part is then added at the end of new_src. If new_src doesn't    */
/* exist then it is created and it will contained the value obtained          */
/* Inputs :                                                                   */
/*  - char **new_src : a pointer to the sub-string that defines the token     */
/*  - t_token *t : a structure that contained datas about the current token   */
/*  - int *d : array of datas about internal criteria of token                */
/*  - void *param : void variable that will be convert into a struct t_data   */
/* Return :                                                                   */
/*  - 0 : if everything goes well                                             */
/*  - int : the error code of the problem encounter                           */
/* ************************************************************************** */
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

/* ************************************************************************** */
/*                        get_new_src_after_expansion                         */
/* -------------------------------------------------------------------------- */
/* This function adds the string that follow a valid expansion at the end of  */
/* the new_src created                                                        */
/* rq : if we have '$<var_name>' then no expansion is expected so we write    */
/* the string contained inside the single quote as it is                      */
/* rq : by combining this function and the previous two ones we will obtained */
/* the full expanded token                                                    */
/* Inputs :                                                                   */
/*  - char **new_src : a pointer to the sub-string that defines the token     */
/*  - t_token *t : a structure that contained datas about the current token   */
/*  - int *d : array of datas about internal criteria of token                */
/* Return :                                                                   */
/*  - 0 : if everything goes well                                             */
/*  - int : the error code of the problem encounter                           */
/* ************************************************************************** */
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

/* ************************************************************************** */
/*                          loop_perform_expansions                           */
/* -------------------------------------------------------------------------- */
/* This function expands every expandable part and replace them by their      */
/* expanded value to obtained the full token                                  */
/* To do so, this fonction combines two part of the token at a time until     */
/* every parts had be treated                                                 */
/*   - if the part is expandable then a string that contained the expanded    */
/* value will be generate and add at the end of new_src                       */
/*   - if the part is non-expandable then this part will be add at the end of */
/* new_src                                                                    */
/* rq : if new_src as yet to be created then the first part encounter will    */
/* become new_src                                                             */
/* Inputs :                                                                   */
/*  - char **new_src : a pointer to the sub-string that defines the token     */
/*  - int *d : array of datas about internal criteria of token                */
/*  - t_token *t : a structure that contained datas about the current token   */
/*  - void *param : void variable that will be convert into a struct t_data   */
/* Return :                                                                   */
/*  - 0 : if everything goes well                                             */
/*  - int : the error code of the problem encounter                           */
/* ************************************************************************** */
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
