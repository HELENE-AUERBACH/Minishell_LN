/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   files_management_2_minishell.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmorice <rmorice@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 20:53:05 by hauerbac          #+#    #+#             */
/*   Updated: 2024/09/23 13:25:00 by rmorice          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_minishell.h"

/* ************************************************************************** */
/*                           cast_dll_el_into_token                           */
/* -------------------------------------------------------------------------- */
/* This function converts the double list element into a structure t_token    */
/* Input :                                                                    */
/*  - t_dll_el **current      */
/* Return :                                                                   */
/*  - NULL : if input t_dll_el doesn't exist or is void                       */
/*  - t_token : the converted result otherwise                                */
/* ************************************************************************** */
// ??? Is it really what this function is doing ???
t_token	*cast_dll_el_into_token(t_dll_el **current)
{
	if (!(current && *current))
		return (NULL);
	return ((t_token *)((*current)->content));
}

/* ************************************************************************** */
/*                            close_previous_file                             */
/* -------------------------------------------------------------------------- */
/* This function closes fd1 and fd2 (of the files opened)                     */
/* Inputs :                                                                   */
/*  - t_dll_el *prev      */
/* Return :                                                                   */
/*  - 0 : if everything goes well                                             */
/*  - int : otherwise                                                         */
/* ************************************************************************** */
// ???
// if fd1 couldn't be close we don't even try to close fd2...
int	close_previous_file(t_dll_el *prev)
{
	t_token	*p;

	if (!(prev && prev->content))
		return (0);
	p = (t_token *)(prev->content);
	if (p->cmd_d && p->cmd_d->fd1 != -1 && close(p->cmd_d->fd1) == -1)
	{
		if (p->cmd_d->is_here_doc == 0)
			display_err_with_prefix(p->cmd_d->file1, \
					" input file close error\n");
		else
			display_error("Temporary file close error\n");
		return (-1);
	}
	p->cmd_d->fd1 = -1;
	if (p->cmd_d && p->cmd_d->fd2 != -1 && close(p->cmd_d->fd2) == -1)
	{
		display_err_with_prefix(p->cmd_d->file2,
			" output file close error\n");
		return (-2);
	}
	p->cmd_d->fd2 = -1;
	return (0);
}


/* ************************************************************************** */
/*                            is_file_name_in_list                            */
/* -------------------------------------------------------------------------- */
/* This function checks if the file_name is present in lst                    */
/* Inputs :                                                                   */
/*  - char *file_name : the file_name that we are looking for                 */
/*  - int file_name_len : the length of the file_name string                  */
/*  - t_list *lst   */
/* Return :                                                                   */
/*  - 1 : if file_name is in lst                                              */
/*  - 0 : otherwise                                                           */
/* ************************************************************************** */
// ???? what does lst contained ????
static int	is_file_name_in_list(char *file_name, int file_name_len,
			t_list *lst)
{
	t_list	*current;

	if (lst)
	{
		current = lst;
		while (current)
		{
			if (ft_strncmp(file_name, (char *) current->content, \
				file_name_len) == 0)
				return (1);
			current = current->next;
		}
	}
	return (0);
}

/* ************************************************************************** */
/*                            check_in_redir_file                             */
/* -------------------------------------------------------------------------- */
/* This function checks if we have an "in" type kind of redirection. If that  */
/* is the case, we check if the file exist and if we can read it.             */
/* Then, we check if we have in_is_pipe and if src is into new_files but not
/* into cmd_new_files => error ??? */
/*  */
/* Inputs :                                                                   */
/*  - t_token *t : a structure that contained datas about the current token   */
/*  - t_list *new_files  */
/*  - t_list *cmd_new_files      */
/*  - int is_in_piped  */
/* Return :                                                                   */
/*  - 0 : if everything goes well                                             */
/*  - int : otherwise                                                         */
/* ************************************************************************** */
int	check_in_redir_file(t_token *t, t_list *new_files,
		t_list *cmd_new_files, int is_in_piped)
{
	if (!(t && t->type == S_IN_REDI))
		return (-1);
	if (access(t->src, F_OK) == -1 || access(t->src, R_OK) == -1)
		return (perror(t->src), -2);
	if (is_in_piped == 1 \
		&& is_file_name_in_list(t->src, t->src_len, new_files) == 1 \
		&& is_file_name_in_list(t->src, t->src_len, cmd_new_files) == 0)
		return (write(2, t->src, t->src_len), \
			write(2, ": No such file or directory\n", 28), -3);
	return (0);
}

/* ************************************************************************** */
/*                            check_out_redir_file                            */
/* -------------------------------------------------------------------------- */
/* This function checks if we have an "out" type kind of redirection. If that */
/* is the case, and the file exists we check if we can write in it            */
/* Input :                                                                    */
/*  - t_token *t : a structure that contained datas about the current token   */
/* Return :                                                                   */
/*  - 0 : if everything goes well                                             */
/*  - int : otherwise                                                         */
/* ************************************************************************** */
int	check_out_redir_file(t_token *t)
{
	if (!(t && (t->type == S_OUT_REDI || t->type == D_OUT_REDI)))
		return (-1);
	if (access(t->src, F_OK) == 0 && access(t->src, W_OK) == -1)
		return (perror(t->src), -2);
	return (0);
}
