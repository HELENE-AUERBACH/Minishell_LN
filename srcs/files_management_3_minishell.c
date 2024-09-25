/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   files_management_3_minishell.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmorice <rmorice@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 14:56:52 by hauerbac          #+#    #+#             */
/*   Updated: 2024/09/25 10:23:40 by rmorice          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* ************************************************************************** */
/*                        open_out_file_and_close_prev                        */
/* -------------------------------------------------------------------------- */
/* This function closes files previously open and opens the new outfile       */
/* needed                                                                     */
/* Inputs :                                                                   */
/*  - t_list **new_files : ptr to lst of files created for * cmd of cmd line  */
/*  - t_list **cmd_new_files : ptr to lst of files created for current cmd    */
/*  - t_token *t : a structure that contained datas about the current token   */
/*  - t_dll_el *prev : previous element of double list relatives to cmd datas */
/* Return :                                                                   */
/*  - 0 : if everything goes well                                             */
/*  - int : the error code of the problem encounter                           */
/* ************************************************************************** */
static int	open_out_file_and_close_prev(t_list **new_files,
			t_list **cmd_new_files, t_token *t, t_dll_el *prev)
{
	char	*file_name;

	if (!(t && new_files && cmd_new_files))
		return (-1);
	if (prev && close_previous_file(prev) < 0)
		return (-2);
	file_name = NULL;
	if (t->type == S_OUT_REDI || t->type == D_OUT_REDI)
	{
		file_name = (char *) malloc((t->src_len + 1) * sizeof(char));
		if (!file_name)
			return (-5);
		ft_strlcpy(file_name, t->src, t->src_len + 1);
		if (open_file(new_files, cmd_new_files, t, file_name) < 0)
			return (-6);
	}
	file_name = NULL;
	return (0);
}

/* ************************************************************************** */
/*                      check_files_for_out_redirections                      */
/* -------------------------------------------------------------------------- */
/* This function cleans, for every output redirection,  the string "t->src"   */
/* by removing spaces present at the beginning of src as well as the external */
/* quotes (if needed)                                                         */
/* Then we check if we have an output redirection and if the file concerned   */
/* exist and if we have the needed rights                                     */
/* if files where previously open they are closed before we open the new      */
/* needed files                                                               */
/* If the output file doesn't already exist then its name is added at the and */
/* new_file an at the end of cmd_new_files lists                              */
/* Inputs :                                                                   */
/*  - t_dll_el **current : pointer to an elt of double list about cmd datas   */
/*  - t_list **new_files : ptr to lst of files created for * cmd of cmd line  */
/*  - t_list **cmd_new_files : ptr to lst of files created for current cmd    */
/*  - int redir_error_first_position : position of the first redir error      */
/* rq : if no such redirection exists, redir_error_first_position = 0         */
/* Return :                                                                   */
/*  - 0 : if everything goes well                                             */
/*  - int : the error code of the problem encounter                           */
/* ************************************************************************** */
int	check_files_for_out_redirections(t_dll_el **current,
		t_list **new_files, t_list **cmd_new_files,
		int redir_error_first_position)
{
	t_token		*t;
	int			result;

	t = cast_dll_el_into_token(current);
	while (t && (t->type == S_OUT_REDI || t->type == D_OUT_REDI)
		&& (redir_error_first_position == 0
			|| t->position < redir_error_first_position))
	{
		result = remove_first_spaces_and_ext_quotes(t);
		if (result != 0)
			return (result);
		if (check_out_redir_file(t) < 0)
			return (-1 * t->position);
		if (open_out_file_and_close_prev(new_files, cmd_new_files, t, \
			(*current)->prev) < 0)
			return (-1 * t->position);
		*current = (*current)->next;
		if (*current)
			t = (t_token *)((*current)->content);
		else
			t = NULL;
	}
	return (0);
}
