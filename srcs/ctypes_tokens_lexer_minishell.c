/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ctypes_tokens_lexer_minishell.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 16:57:16 by hauerbac          #+#    #+#             */
/*   Updated: 2024/05/07 15:29:26 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_minishell.h"

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
