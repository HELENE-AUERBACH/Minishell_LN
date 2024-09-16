/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_tokens_3_minishell.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmorice <rmorice@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 14:49:33 by hauerbac          #+#    #+#             */
/*   Updated: 2024/09/16 17:12:48 by rmorice          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* ************************************************************************** */
/*                             set_last_redir_in                              */
/* -------------------------------------------------------------------------- */
/* This function takes the variables file1 and is_here_doc and saves them in  */
/* the structure t_cmdbi                                                      */
/* Inputs :                                                                   */
/*  - t_token *t_cmdbi      */
/*  - char *file1 : a string that contained input file name                   */
/*  - int is_here_doc : an int that specified if we are in an heredoc context */
/* Return :                                                                   */
/*  - 0 : if everything goes well                                             */
/*  - 3 : otherwise (malloc error)                                            */
/* ************************************************************************** */
// why do we, once more, take existing datas and copying them in another struct
// plus it is the exact same variables as those that we copy just before... ???
static int	set_last_redir_in(t_token *t_cmdbi, char *file1,
			int is_here_doc)
{
	int	file1_len;

	if (file1)
	{
		file1_len = ft_strlen(file1);
		t_cmdbi->cmd_d->file1 = (char *) malloc((file1_len + 1) \
			* sizeof(char));
		if (!t_cmdbi->cmd_d->file1)
			return (3);
		ft_strlcpy(t_cmdbi->cmd_d->file1, file1, file1_len + 1);
		t_cmdbi->cmd_d->is_here_doc = is_here_doc;
	}
	return (0);
}

/* ************************************************************************** */
/*                             set_last_redir_in                              */
/* -------------------------------------------------------------------------- */
/* This function takes the variables file2 and is_fd2_o_append and saves them */
/* in the structure t_cmdbi                                                   */
/* Inputs :                                                                   */
/*  - t_token *t_cmdbi      */
/*  - char *file2 : a string that contained output file name                  */
/*  - int is_fd2_o_append : specified if fd2 was open in append mode          */
/* Return :                                                                   */
/*  - 0 : if everything goes well                                             */
/*  - 3 : otherwise (malloc error)                                            */
/* ************************************************************************** */
static int	set_last_redir_out(t_token *t_cmdbi, char *file2,
			int is_fd2_o_append)
{
	int	file2_len;

	if (file2)
	{
		file2_len = ft_strlen(file2);
		t_cmdbi->cmd_d->file2 = (char *) malloc((file2_len + 1) \
			* sizeof(char));
		if (!t_cmdbi->cmd_d->file2)
			return (3);
		ft_strlcpy(t_cmdbi->cmd_d->file2, file2, file2_len + 1);
		t_cmdbi->cmd_d->is_fd2_o_append = is_fd2_o_append;
	}
	return (0);
}

/* ************************************************************************** */
/*                              set_in_variables                              */
/* -------------------------------------------------------------------------- */
/* This function checks if the token type is one of input redirection. If     */
/* that is the case then it set file1 and is_here_doc to their respective     */
/* content from t->cmd_d                                                      */
/* Inputs :                                                                   */
/*  - t_token *t      */
/*  - char **file1 : a pointer to a string that contained input file name     */
/*  - int *is_here_doc : pointer to a var that keep if token was heredoc type */
/* Return :                                                                   */
/*  - None                                                                    */
/* ************************************************************************** */
static void	set_in_variables(t_token *t, char **file1, int *is_here_doc)
{
	if (t->type == S_IN_REDI || t->type == HEREDOC)
	{
		*file1 = t->cmd_d->file1;
		*is_here_doc = t->cmd_d->is_here_doc;
	}
}

/* ************************************************************************** */
/*                             set_out_variables                              */
/* -------------------------------------------------------------------------- */
/* This function checks if the token type is one of output redirection. If    */
/* that is the case then it set file2 and is_fd2_o_append to their respective */
/* content from t->cmd_d                                                      */
/* Inputs :                                                                   */
/*  - t_token *t      */
/*  - char **file2 : a pointer to a string that contained output file name    */
/*  - int *is_fd2_o_append : pointer to var that keep if open in append mode  */
/* Return :                                                                   */
/*  - None                                                                    */
/* ************************************************************************** */
static void	set_out_variables(t_token *t, char **file2,
			int *is_fd2_o_append)
{
	if (t->type == S_OUT_REDI || t->type == D_OUT_REDI)
	{
		*file2 = t->cmd_d->file2;
		*is_fd2_o_append = t->cmd_d->is_fd2_o_append;
	}
}

/* ************************************************************************** */
/*                         set_last_redir_files_names                         */
/* -------------------------------------------------------------------------- */
/* This function copies in t_cmdbi the datas relatives to the last input and  */
/* output redirection file name and type of redirection                       */
/* Inputs :                                                                   */
/*  - t_token **t_cmdbi      */
/*  - t_dll_el *start_el : a pointer to the first element to process          */
/* Return :                                                                   */
/*  - 0 : if everything goes well                                             */
/*  - int : the error code of the problem encounter                           */
/* ************************************************************************** */
int	set_last_redir_files_names(t_token *t_cmdbi, t_dll_el *start_el)
{
	t_token	*t;
	char	*file1;
	char	*file2;
	int		is_here_doc;
	int		is_fd2_o_append;

	if (!(t_cmdbi && t_cmdbi->cmd_d && start_el && start_el->content))
		return (-1);
	file1 = NULL;
	file2 = NULL;
	is_here_doc = 0;
	is_fd2_o_append = 0;
	t = (t_token *) start_el->content;
	while (t && t != t_cmdbi)
	{
		set_in_variables(t, &file1, &is_here_doc);
		set_out_variables(t, &file2, &is_fd2_o_append);
		t = NULL;
		start_el = start_el->next;
		if (start_el && start_el->content)
			t = (t_token *) start_el->content;
	}
	if (set_last_redir_in(t_cmdbi, file1, is_here_doc) == 0)
		return (set_last_redir_out(t_cmdbi, file2, is_fd2_o_append));
	return (3);
}
