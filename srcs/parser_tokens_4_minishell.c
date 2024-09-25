/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_tokens_4_minishell.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmorice <rmorice@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 13:37:18 by hauerbac          #+#    #+#             */
/*   Updated: 2024/09/25 12:26:38 by rmorice          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* ************************************************************************** */
/*                        force_in_redir_of_next_cmdbi                        */
/* -------------------------------------------------------------------------- */
/* This function specifies, if needed, the "infile" of the input redirection  */
/* to "/dev/null"                                                             */
/* It is done under specific conditions : no error has been encounter, the    */
/* command is in pipe, no input file has been specified and the variable      */
/* next_has_to_be_forced is set to 1                                          */
/* A redirection has to be forced when we have an input pipe without an input */
/* file (file1)                                                               */
/* Inputs :                                                                   */
/*  - t_token *t_cmdbi : a struct that contained datas about cmd or builtin   */
/*  - int *next_has_to_be_forced : specified if the input has to be forced    */
/*  - int *result : pointer to an int that specified the prev encountered err */
/* Return :                                                                   */
/*  - 0 : if everything goes well                                             */
/*  - int : the error code of the problem encounter                           */
/* ************************************************************************** */
void	force_in_redir_of_next_cmdbi(t_token *t_cmdbi,
		int *next_has_to_be_forced, int *result)
{
	if (t_cmdbi && (t_cmdbi->type == COMMAND || t_cmdbi->type == BI) \
		&& t_cmdbi->cmd_d)
	{
		if (t_cmdbi->cmd_d->is_in_piped == 1 \
			&& *next_has_to_be_forced == 1 \
			&& g_exit_status != 130 \
			&& !t_cmdbi->cmd_d->file1 \
			&& !(*result == 3 || *result == -3))
		{
			t_cmdbi->cmd_d->file1 = (char *) malloc(sizeof(char) \
									* 10);
			if (!t_cmdbi->cmd_d->file1)
				*result = -3;
			ft_strlcpy(t_cmdbi->cmd_d->file1, "/dev/null", 10);
		}
		*next_has_to_be_forced = 0;
	}
}
