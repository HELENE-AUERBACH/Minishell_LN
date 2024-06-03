/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_minishell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 14:15:31 by hauerbac          #+#    #+#             */
/*   Updated: 2024/06/03 13:54:58 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	free_cmd_d(t_cmd *cmd_d)
{
	if (cmd_d)
	{
		if (cmd_d->file1)
			close_in_file_and_free_file_name(cmd_d);
		cmd_d->file1 = NULL;
		if (cmd_d->file2)
			close_out_file_and_free_file_name(cmd_d);
		cmd_d->file2 = NULL;
		if (cmd_d->limiter)
			free(cmd_d->limiter);
		cmd_d->limiter = NULL;
		if (cmd_d->cmd)
			free(cmd_d->cmd);
		cmd_d->cmd = NULL;
		if (cmd_d->paths_tab)
			free_tab(&cmd_d->paths_tab);
		if (cmd_d->args)
			free_tab(&cmd_d->args);
		free(cmd_d);
	}
	return ;
}

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
			free_cmd_d(token->cmd_d);
		token->cmd_d = NULL;
		free(token);
		content = NULL;
	}
	return ;
}

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
		dll_clear(&d->lst, del_el_content);
	}
}
