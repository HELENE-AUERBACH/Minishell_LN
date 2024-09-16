/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_tokens_2_minishell.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmorice <rmorice@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 14:07:08 by hauerbac          #+#    #+#             */
/*   Updated: 2024/09/12 12:06:36 by rmorice          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* ************************************************************************** */
/*                          get_substr_after_spaces                           */
/* -------------------------------------------------------------------------- */
/* This function looks for the firs character that isn't a space. The index   */
/* of this char will determined the start of the substring that will be the   */
/* new "src". The substring is src without the spaces at the beginning of src */
/* (if src doesn't start with spaces then substring will be a copy of src)    */
/* Inputs :                                                                   */
/*  - t_token *t :      */
/*  - int *utils_d :    */
/* Return :                                                                   */
/*  - 0 : if everything goes well                                             */
/*  - 3 : otherwise (malloc error)                                            */
/* ************************************************************************** */
static int	get_substr_after_spaces(t_token *t, int *utils_d)
{
	int		i;
	char	*s;

	i = 0;
	while (is_a_space(t->src[i]))
		i++;
	if (i > 0)
	{
		utils_d[START] = i;
		s = ft_substr(t->src, utils_d[START],
				t->src_len - utils_d[START]);
		if (!s)
			return (3);
		free(t->src);
		t->src = s;
		t->src_len = ft_strlen(s);
		utils_d[STR_LEN] = t->src_len;
		s = NULL;
	}
	return (0);
}

/* ************************************************************************** */
/*                     remove_first_spaces_and_ext_quotes                     */
/* -------------------------------------------------------------------------- */
/* This function removes, if needed, the starting spaces as well as the       */
/* eternals quotes symboles                                                   */
/* (we assumed that the quote symbole is the first char after spaces, that it */
/* is closed and that the last character before '\0' is the closing quote     */
/* symbole)                                                                   */
/* To be sure we check that the substring obtained is well formated           */
/* Input :                                                                    */
/*  - t_token *t        */
/* Return :                                                                   */
/*  - 0 : if everything goes well                                             */
/*  - int : otherwise                                                         */
/* ************************************************************************** */
int	remove_first_spaces_and_ext_quotes(t_token *t)
{
	int		utils_d[9];
	char	*s;

	init_utils_data(utils_d, t->src_len);
	if (get_substr_after_spaces(t, utils_d) == 3)
		return (3);
	check_simple_quotes(0, utils_d, t->src);
	check_double_quotes(0, utils_d, t->src);
	if (utils_d[EXT_OPEN_S_QUOTE] == 1
		|| utils_d[EXT_OPEN_D_QUOTE] == 1)
	{
		utils_d[START] = 1;
		s = ft_substr(t->src, 1, t->src_len - 2);
		if (!s)
			return (3);
		free(t->src);
		t->src = s;
		t->src_len = ft_strlen(s);
		utils_d[STR_LEN] = t->src_len;
		s = NULL;
	}
	if ((t->type == COMMAND || t->type == BI) && t->src_len > 1
		&& has_wrong_start(t->src[0], '\0'))
		return (2);
	return (0);
}

/* ************************************************************************** */
/*                         is_a_pipe_or_ctrloperator                          */
/* -------------------------------------------------------------------------- */
/* This function checks if the type received is a pipe ("|") or a control     */
/* operator (";", "&", double ";;", "&&", "||", ";&", ";;&", "|&")            */
/* Input :                                                                    */
/*  - enum e_token_types type : the type to check                             */
/* Return :                                                                   */
/*  - 1 : if the type received is a pipe or a control operator                */
/*  - 0 : otherwise                                                           */
/* ************************************************************************** */
int	is_a_pipe_or_ctrloperator(enum e_token_types type)
{
	if (type == PIPE || type == SEMICOLON || type == AMPERSAND
		|| type == DOUBLE_SEMICOLON || type == LOGICAL_AND
		|| type == LOGICAL_OR || type == SEMICOLON_AMPERSAND
		|| type == DOUBLE_SEMICOLON_AMPERSAND || type == PIPE_AMPERSAND)
		return (1);
	return (0);
}

/* ************************************************************************** */
/*                                   ft_min                                   */
/* -------------------------------------------------------------------------- */
/* This function returns the minimal value between the two integrals values   */
/* given in input                                                             */
/* Inputs :                                                                   */
/*  - int value1 : the first value to compare                                 */
/*  - int value2 : the second value to compare                                */
/* Return :                                                                   */
/*  - int : the minimal value between value1 and value2                       */
/* ************************************************************************** */
static int	ft_min(int value1, int value2)
{
	if (value1 <= value2)
		return (value1);
	else
		return (value2);
}

/* ************************************************************************** */
/*                             check_redir_files                              */
/* -------------------------------------------------------------------------- */
/* This function checks for each redirection (in or out) if we have access to */
/* it and if we have the needed rights. Then ??? (cast token) */
/* If we already have open one file for the same type of redirection then we  */
/* close the one previous open to be ready to open the new file               */
/* Inputs :                                                                   */
/*  - t_dll_el **current      */
/*  - t_data *d : a structure that contained infos relative to the shell      */
/*  - int is_in_piped      */
/* Return :                                                                   */
/*  - 0 : if everything goes well                                             */
/*  - int : the error code of the problem encounter                           */
/* ************************************************************************** */
int	check_redir_files(t_dll_el **current, t_data *d, int is_in_piped)
{
	int			redir_err;
	int			redir_err1;
	t_token		*t;

	if (!(current && *current && d))
		return (1);
	redir_err = 0;
	while (current && *current && (is_an_out_redirect((*current)->content) \
		|| is_an_in_redirect((*current)->content)))
	{
		redir_err1 = ft_min(check_files_for_in_redirections(current, \
				-1 * redir_err, d, is_in_piped), redir_err);
		redir_err = ft_min(check_files_for_out_redirections(current, \
		&d->new_files, &d->cmd_new_files, -1 * redir_err1), redir_err1);
		t = cast_dll_el_into_token(current);
		if (t && ((redir_err1 < 0 && t->position >= -1 * redir_err1) \
			|| (redir_err < 0 && t->position >= -1 * redir_err)))
			break ;
	}
	if (current && *current && (*current)->prev
		&& (is_an_out_redirect((*current)->prev->content) == 1
			|| is_an_in_redirect((*current)->prev->content) == 1))
		close_previous_file((*current)->prev);
	return (redir_err);
}
