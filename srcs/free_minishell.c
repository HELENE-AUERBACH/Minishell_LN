/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_minishell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmorice <rmorice@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 14:15:31 by hauerbac          #+#    #+#             */
/*   Updated: 2024/09/23 13:35:13 by rmorice          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* ************************************************************************** */
/*                             free_tab_with_size                             */
/* -------------------------------------------------------------------------- */
/* This function frees the size first elements of array tab then free tab     */
/* (the expected size value is the size f the array)                          */
/* Inputs :                                                                   */
/*  - char ***tab : a pointer t th first element of an array to free          */
/*  - int size : the number of element to free                                */
/* Return :                                                                   */
/*  - None                                                                    */
/* ************************************************************************** */
void	free_tab_with_size(char ***tab, int size)
{
	int	i;

	if (*tab)
	{
		i = 0;
		while (i < size && (*tab)[i])
		{
			free((*tab)[i]);
			(*tab)[i] = NULL;
			i++;
		}
		free(*tab);
		*tab = NULL;
	}
}

/* ************************************************************************** */
/*                                  free_tab                                  */
/* -------------------------------------------------------------------------- */
/* This function frees each elements of array tab and make it points toward   */
/* NULL then free tab                                                         */
/* Input :                                                                    */
/*  - char ***tab : a pointer t th first element of an array to free          */
/* Return :                                                                   */
/*  - None                                                                    */
/* ************************************************************************** */
void	free_tab(char ***tab)
{
	int	i;

	if (*tab)
	{
		i = 0;
		while ((*tab)[i])
		{
			free((*tab)[i]);
			(*tab)[i] = NULL;
			i++;
		}
		free(*tab);
		*tab = NULL;
	}
}

/* ************************************************************************** */
/*                                 free_cmd_d                                 */
/* -------------------------------------------------------------------------- */
/* This function closes the infile (temporary file if heredoc) and frees the  */
/* string that contained the file name associated is needed. The same thing   */
/* is done for the outfile if needed                                          */
/* Variables associated to the command, its arguments and its path are also   */
/* freed and cmd_d is set to point toward NULL                                */
/* rq : In the case of a "heredoc", the file is unlink (disappear after use)  */
/* and the variable that contained the limiter is freed and is set to point   */
/* toward NULL                                                                */
/* Input :                                                                    */
/*  - t_cmd **cmd_d : a pointer to a struct that contained datas about a cmd  */
/* Return :                                                                   */
/*  - None                                                                    */
/* ************************************************************************** */
void	free_cmd_d(t_cmd **cmd_d)
{
	if (*cmd_d)
	{
		if ((*cmd_d)->file1)
			close_in_file_and_free_file_name(*cmd_d);
		(*cmd_d)->file1 = NULL;
		if ((*cmd_d)->file2)
			close_out_file_and_free_file_name(*cmd_d);
		(*cmd_d)->file2 = NULL;
		if ((*cmd_d)->limiter)
			free((*cmd_d)->limiter);
		(*cmd_d)->limiter = NULL;
		if ((*cmd_d)->cmd)
			free((*cmd_d)->cmd);
		(*cmd_d)->cmd = NULL;
		if ((*cmd_d)->paths_tab)
			free_tab(&(*cmd_d)->paths_tab);
		if ((*cmd_d)->args)
			free_tab(&(*cmd_d)->args);
		free(*cmd_d);
		*cmd_d = NULL;
	}
}

/* ************************************************************************** */
/*                               del_el_content                               */
/* -------------------------------------------------------------------------- */
/* This function frees every componants (src, cmd_d) that can be free from    */
/* content                                                                    */
/* Input :                                                                    */
/*  - void *content : the element that we want to freed and cleared           */
/* Return :                                                                   */
/*  - None                                                                    */
/* ************************************************************************** */
void	del_el_content(void *content)
{
	t_token	*token;

	if (content)
	{
		token = (t_token *) content;
		if (token->src)
			free(token->src);
		token->src = NULL;
		token->src_len = 0;
		if (token->cmd_d)
			free_cmd_d(&token->cmd_d);
		token->cmd_d = NULL;
		free(token);
		token = NULL;
		content = NULL;
	}
	return ;
}

/* ************************************************************************** */
/*                                 free_data                                  */
/* -------------------------------------------------------------------------- */
/* This function frees d datas as envp, cmds and lst and every freeable       */
/* elements contained.                                                        */
/* Input :                                                                    */
/*  - t_data *d : a structure that contained infos relative to the shell      */
/* Return :                                                                   */
/*  - None                                                                    */
/* ************************************************************************** */
void	free_data(t_data *d)
{
	if (d)
	{
		if (d->envp)
			free_tab_with_size(&d->envp, d->envp_size);
		d->envp_size = 0;
		if (d->cmds)
			ft_lstclear(&d->cmds, del_el_content);
		d->nb_cmds = 0;
		if (d->lst)
			dll_clear(&d->lst, del_el_content);
	}
}
