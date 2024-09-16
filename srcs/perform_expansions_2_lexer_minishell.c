/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   perform_expansions_2_lexer_minishell.c             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmorice <rmorice@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 12:56:53 by hauerbac          #+#    #+#             */
/*   Updated: 2024/09/16 17:29:42 by rmorice          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* ************************************************************************** */
/*                         is_not_a_valid_identifier                          */
/* -------------------------------------------------------------------------- */
/* This function checks if is a "valid identifier" or not.                    */
/* A "valid identifier" is ??????????????? */
/*  */
/* If it isn't a "valid identifier" then an error message is display and the  */
/* function returns 1. Otherwise the return value will be 0                   */
/* Inputs :                                                                   */
/*  - t_token *t :    */
/*  - int i :   */
/*  - int j :   */
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
/* a space then the index of the end of the sub-string  that will become the  */
/* token is incremented to keep the space.                                    */
/* If a sub-string already exist then we add the new substring obtained at    */
/* the end of it. Otherwise we create a new subtring.                         */
/* Inputs :                                                                   */
/*  - char **new_src : a pointer to the sub-string that defines the token     */
/*  - int *d :  */
/*  - t_token *t :  */
/*  - void *param :       */
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
/*  - t_token *t :  */
/*  - int *d :  */
/*  - void *param :       */
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
/* This function  */
/*  */
/*  */
/*  */
/*  */
/* Inputs :                                                                   */
/*  - char **new_src : a pointer to the sub-string that defines the token     */
/*  - t_token *t :  */
/*  - int *d :  */
/* Return :                                                                   */
/*  - 0 : if everything goes well                                             */
/*  - int : the error code of the problem encounter                           */
/* ************************************************************************** */
// ??? s is add at the back of new_src but is contained in s ???
// why do we check double quote in the while but not in the criteria for the while to loop ???
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
/* This function expands   */
/* Inputs :                                                                   */
/*  - char **new_src : a pointer to the sub-string that defines the token     */
/*  - int *d :  */
/*  - t_token *t :  */
/*  - void *param :       */
/* Return :                                                                   */
/*  - 0 : if everything goes well                                             */
/*  - int : the error code of the problem encounter                           */
/* ************************************************************************** */
// I suppose that this fonction combine two part of the token at a time :
//  - if the token start with an expansion then new_src will become the expanded value
//  - if the token start with a non-expandable then new_src will become the part
//    that need not to be expand and the value of the first part that needed to be expand
// as this function is call in a loop then little by little every expansion will be
// done and add at the end of new_src.
// if their is no more expansion to do in the token then the "rest" is added
// Is that it ???
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
