/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_tokens_4_minishell.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 13:37:18 by hauerbac          #+#    #+#             */
/*   Updated: 2024/07/31 17:03:30 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
