/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   files_management_minishell.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmorice <rmorice@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 20:53:05 by hauerbac          #+#    #+#             */
/*   Updated: 2024/09/25 10:24:27 by rmorice          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* ************************************************************************** */
/*                           save_created_file_name                           */
/* -------------------------------------------------------------------------- */
/* This function checks if the file exist. If it doesn't exist then filename  */
/* is added at the end of new_files and at the end of cmd_new_files lists     */
/* Inputs :                                                                   */
/*  - t_list **new_files : ptr to lst of files created for * cmd of cmd line  */
/*  - t_list **cmd_new_files : ptr to lst of files created for current cmd    */
/*  - char *file_name : the name of the file to check and optionally add      */
/* Return :                                                                   */
/*  - 0 : if everything goes well                                             */
/*  - 3 : otherwise (malloc error)                                            */
/* ************************************************************************** */
static int	save_created_file_name(t_list **new_files,
			t_list **cmd_new_files, char *file_name)
{
	t_list	*lst_new;
	t_list	*lst_new_2;

	if (access(file_name, F_OK) == -1)
	{
		lst_new = ft_lstnew((void *) file_name);
		if (!lst_new)
			return (3);
		ft_lstadd_back(new_files, lst_new);
		lst_new_2 = ft_lstnew((void *) file_name);
		if (!lst_new_2)
			return (3);
		ft_lstadd_back(cmd_new_files, lst_new_2);
	}
	return (0);
}

/* ************************************************************************** */
/*                                 open_file                                  */
/* -------------------------------------------------------------------------- */
/* This function opens the file that needed t be open with the expected       */
/* options and save their name and fd (file descriptor) into file1 and fd1    */
/* (if input redirection) or file2 and fd2 (if output redirection)            */
/* First the type of the token is check to deetermined if we are preparing    */
/* for an input redirection oor an output redirection.                        */
/* If the type isn't "heredoc" the the file1 or file2 name is update          */
/* Then the file is open (if output file doesn't exist it is created and add  */
/* to new_files and cmd_new_files lists                                       */
/* Inputs :                                                                   */
/*  - t_list **new_files : ptr to lst of files created for * cmd of cmd line  */
/*  - t_list **cmd_new_files : ptr to lst of files created for current cmd    */
/*  - t_token *t : a structure that contained datas about the current token   */
/*  - char *file_name : the file name of the file that we want to open        */
/* Return :                                                                   */
/*  - 0 : if everything goes well                                             */
/*  - int : the error code of the problem encounter                           */
/* ************************************************************************** */
int	open_file(t_list **new_files, t_list **cmd_new_files, t_token *t,
		char *file_name)
{
	if (t->type == S_IN_REDI)
		t->cmd_d->file1 = file_name;
	if (t->type == S_IN_REDI || t->type == HEREDOC)
	{
		t->cmd_d->fd1 = open(t->cmd_d->file1, O_RDONLY);
		if (t->type == S_IN_REDI && t->cmd_d->fd1 == -1)
			return (display_err_with_prefix(t->cmd_d->file1, \
					" input file open error\n"), -1);
		else if (t->type == HEREDOC && t->cmd_d->fd1 == -1)
			return (display_error("T. input file open err.\n"), -1);
	}
	if (t->type == S_OUT_REDI || t->type == D_OUT_REDI)
	{
		t->cmd_d->file2 = file_name;
		if (save_created_file_name(new_files, cmd_new_files, \
			file_name) != 0)
			return (-2);
	}
	if (t->type == S_OUT_REDI)
		t->cmd_d->fd2 = open(file_name, O_WRONLY | O_CREAT | O_TRUNC,
				0644);
	if (t->type == D_OUT_REDI)
		t->cmd_d->fd2 = open(file_name, O_WRONLY | O_CREAT | O_APPEND,
				0644);
	return (0);
}

/* ************************************************************************** */
/*                        open_in_file_and_close_prev                         */
/* -------------------------------------------------------------------------- */
/* This function closes files previously open, creates if needed a temporary  */
/* file for "heredoc". The input filename (existing file or temporary file)   */
/* is saved and the file in question is open                                  */
/* Inputs :                                                                   */
/*  - t_list **new_files : ptr to lst of files created for * cmd of cmd line  */
/*  - t_list **cmd_new_files : ptr to lst of files created for current cmd    */
/*  - t_dll_el *current : an elt of a double list relative to cmd datas       */
/* Return :                                                                   */
/*  - 0 : if everything goes well                                             */
/*  - int : the error code of the problem encounter                           */
/* ************************************************************************** */
static int	open_in_file_and_close_prev(t_list **new_files,
			t_list **cmd_new_files, t_dll_el *current)
{
	char		*file_name;
	t_token		*t;

	t = cast_dll_el_into_token(&current);
	if (!(t && new_files && cmd_new_files))
		return (-1);
	if (current && current->prev && close_previous_file(current->prev) < 0)
		return (-2);
	if (t->type == HEREDOC && create_heredoc_file(t, current) < 0)
		return (-4);
	file_name = NULL;
	if (t->type == S_IN_REDI)
	{
		file_name = (char *) malloc((t->src_len + 1) * sizeof(char));
		if (!file_name)
			return (-5);
		ft_strlcpy(file_name, t->src, t->src_len + 1);
	}
	if (t->type == HEREDOC)
		file_name = t->cmd_d->file1;
	if (open_file(new_files, cmd_new_files, t, file_name) < 0)
		return (-6);
	if (t->type == HEREDOC)
		return (g_exit_status);
	return (0);
}

/* ************************************************************************** */
/*                      check_files_for_in_redirections                       */
/* -------------------------------------------------------------------------- */
/* This function cleans, for every input redirection,  the string "t->src" by */
/* removing spaces present at the beginning of src as well as the external    */
/* quotes (if needed)                                                         */
/* Then we check if we have an input redirection and if the file concerned    */
/* exist and if we have the needed rights                                     */
/* if files where previously open they are closed before we open the new      */
/* needed files                                                               */
/* Inputs :                                                                   */
/*  - t_dll_el **current : pointer to an elt of double list about cmd datas   */
/*  - int redir_error_first_position : position of the first redir error      */
/* rq : if no such redirection exists, redir_error_first_position = 0         */
/*  - t_data *d : a structure that contained infos relative to the shell      */
/*  - int is_in_piped : specified if the token is preceded by a pipe          */
/* Return :                                                                   */
/*  - 0 : if everything goes well                                             */
/*  - int : the error code of the problem encounter                           */
/* ************************************************************************** */
int	check_files_for_in_redirections(t_dll_el **current,
		int redir_error_first_position, t_data *d, int is_in_piped)
{
	t_token		*t;
	int			result;

	t = cast_dll_el_into_token(current);
	while (t && (t->type == S_IN_REDI || t->type == HEREDOC)
		&& (redir_error_first_position == 0
			|| t->position < redir_error_first_position))
	{
		result = remove_first_spaces_and_ext_quotes(t);
		if (result != 0)
			return (result);
		if (t->type == S_IN_REDI
			&& (check_in_redir_file(t, d->new_files, \
				d->cmd_new_files, is_in_piped) < 0))
			return (-1 * t->position);
		if (open_in_file_and_close_prev(&d->new_files, \
			&d->cmd_new_files, *current) != 0)
			return (-1 * t->position);
		*current = (*current)->next;
		if (*current)
			t = (t_token *)((*current)->content);
		else
			t = NULL;
	}
	return (0);
}
