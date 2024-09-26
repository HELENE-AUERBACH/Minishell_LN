/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_2_minishell.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmorice <rmorice@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 14:15:31 by hauerbac          #+#    #+#             */
/*   Updated: 2024/09/26 14:28:05 by rmorice          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* ************************************************************************** */
/*              close_in_file_and_free_file_name_without_unlink               */
/* -------------------------------------------------------------------------- */
/* This function closes the input file (temporary file if pseudo-heredoc)     */
/* opened, frees the variable associated to this file name and makes it point */
/* toward NULL. The fd1 (input file descriptor) variable is set to -1.        */
/* If an error occured then an error message is displayed.                    */
/* Input :                                                                    */
/*  - t_cmd *cmd_d : a struct that contained datas relative to a specific cmd */
/* Return :                                                                   */
/*  - None                                                                    */
/* ************************************************************************** */
static void	close_in_file_and_free_file_name_without_unlink(t_cmd *cmd_d)
{
	if (cmd_d)
	{
		if (cmd_d->is_here_doc && cmd_d->fd1 != -1
			&& close(cmd_d->fd1) == -1)
		{
			perror("Temporary input file close error");
		}
		else if (!cmd_d->is_here_doc && cmd_d->fd1 != -1
			&& close(cmd_d->fd1) == -1)
		{
			perror("Input file close error");
			write(2, cmd_d->file1, ft_strlen(cmd_d->file1));
			write(2, "\n", 1);
		}
		else
			cmd_d->fd1 = -1;
		if (cmd_d->file1)
			free(cmd_d->file1);
		cmd_d->file1 = NULL;
	}
}

/* ************************************************************************** */
/*              close_files_and_free_files_names_without_unlink               */
/* -------------------------------------------------------------------------- */
/* This function closes the input and / or output file(s) opened, frees the   */
/* variable(s) associated to this / these file(s) name and makes it / them    */
/* point toward NULL. The fd1 (input file descriptor) and / or fd2 (output    */
/* file descriptor) variable is / are set to -1.                              */
/* If an error occured then an error message is displayed.                    */
/* rq : In the case of a pseudo-heredoc, the variable associated to the       */
/* limiter is free and make to point toward NULL                              */
/* Input :                                                                    */
/*  - t_cmd *cmd_d : a struct that contained datas relative to a specific cmd */
/* Return :                                                                   */
/*  - None                                                                    */
/* ************************************************************************** */
void	close_files_and_free_files_names_without_unlink(t_cmd *cmd_d)
{
	if (cmd_d)
	{
		if (cmd_d->file1)
			close_in_file_and_free_file_name_without_unlink(cmd_d);
		cmd_d->file1 = NULL;
		if (cmd_d->file2)
			close_out_file_and_free_file_name(cmd_d);
		cmd_d->file2 = NULL;
		if (cmd_d->limiter)
			free(cmd_d->limiter);
		cmd_d->limiter = NULL;
	}
	return ;
}
