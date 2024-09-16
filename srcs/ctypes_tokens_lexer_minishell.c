/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ctypes_tokens_lexer_minishell.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmorice <rmorice@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 16:57:16 by hauerbac          #+#    #+#             */
/*   Updated: 2024/09/13 12:16:56 by rmorice          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_minishell.h"

////////////////////////////////////////////////////////////////////////////////
//                                    ???                                     //
// -------------------------------------------------------------------------- //
// is that used anywhere ??? If yes, where ?                                  //
////////////////////////////////////////////////////////////////////////////////
int	is_a_cmdbi_or_outredirection_dll_el(void *el_content)
{
	t_token	*token;

	if (el_content)
	{
		token = (t_token *) el_content;
		if (token->type == S_OUT_REDI || token->type == D_OUT_REDI
			|| token->type == COMMAND || token->type == BI)
			return (1);
	}
	return (0);
}

////////////////////////////////////////////////////////////////////////////////
//                                    ???                                     //
// -------------------------------------------------------------------------- //
// is that used anywhere ??? If yes, where ?                                  //
////////////////////////////////////////////////////////////////////////////////
int	is_a_cmd_or_bi_dll_el(void *el_content)
{
	t_token	*token;

	if (el_content)
	{
		token = (t_token *) el_content;
		if (token->type == COMMAND || token->type == BI)
			return (1);
	}
	return (0);
}

/* ************************************************************************** */
/*                             is_an_out_redirect                             */
/* -------------------------------------------------------------------------- */
/* This function checks if the token received is an output redirection        */
/* to do so, we look at the type of the token                                 */
/* Input :                                                                    */
/*  - void *el_content :     */
/* Return :                                                                   */
/*  - 1 : if the token is of type S_OUT_REDI or D_OUT_REDIR                   */
/*  - 0 : otherwise                                                           */
/* ************************************************************************** */
int	is_an_out_redirect(void *el_content)
{
	t_token	*token;

	if (el_content)
	{
		token = (t_token *) el_content;
		if (token->type == S_OUT_REDI || token->type == D_OUT_REDI)
			return (1);
	}
	return (0);
}

/* ************************************************************************** */
/*                             is_an_in_redirect                              */
/* -------------------------------------------------------------------------- */
/* This function checks if the token received is an input redirection         */
/* to do so, we look at the type of the token                                 */
/* Input :                                                                    */
/*  - void *el_content :     */
/* Return :                                                                   */
/*  - 1 : if the token is of type S_IN_REDI or D_IN_REDIR                     */
/*  - 0 : otherwise                                                           */
/* ************************************************************************** */
int	is_an_in_redirect(void *el_content)
{
	t_token	*token;

	if (el_content)
	{
		token = (t_token *) el_content;
		if (token->type == S_IN_REDI || token->type == HEREDOC)
			return (1);
	}
	return (0);
}
