/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_tokens_3_minishell.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 14:49:33 by hauerbac          #+#    #+#             */
/*   Updated: 2024/05/16 12:51:46 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static void	set_in_variables(t_token *t, char **file1, int *is_here_doc)
{
	if (t->type == S_IN_REDI || t->type == HEREDOC)
	{
		*file1 = t->cmd_d->file1;
		*is_here_doc = t->cmd_d->is_here_doc;
	}
}

static void	set_out_variables(t_token *t, char **file2,
			int *is_fd2_o_append)
{
	if (t->type == S_OUT_REDI || t->type == D_OUT_REDI)
	{
		*file2 = t->cmd_d->file2;
		*is_fd2_o_append = t->cmd_d->is_fd2_o_append;
	}
}

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
