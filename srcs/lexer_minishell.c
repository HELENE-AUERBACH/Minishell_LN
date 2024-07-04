/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_minishell.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 14:07:09 by hauerbac          #+#    #+#             */
/*   Updated: 2024/07/04 14:34:38 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_minishell.h"

void	init_utils_data_and_new_src(int *utils_data, char **new_src,
		int str_len)
{
	init_utils_data(utils_data, str_len);
	*new_src = NULL;
}

int	get_end_index_of_file_or_delimiter_name(int *i, const char *str)
{
	int	j;

	if (i && *i >= 0 && str && str[*i] && str[*i] == '\\')
		(*i)++;
	j = *i;
	if (str && j >= 0 && str[j] && str[j] == '"')
	{
		j++;
		while (str[j] && str[j] != '"')
			j++;
	}
	else if (str && j >= 0 && str[j] && str[j] == '\'')
	{
		j++;
		while (str[j] && str[j] != '\'')
			j++;
	}
	if (str && j >= 0 && str[j] && (str[j] == '"' || str[j] == '\''))
		j++;
	else if (str && j >= 0)
	{
		while (!is_a_metacharacter(str[j]))
			j++;
	}
	return (j);
}

int	init_token_cmd_d(t_token *token)
{
	token->cmd_d = (t_cmd *) malloc(sizeof(t_cmd));
	if (!token->cmd_d)
		return (-3);
	token->cmd_d->file1 = NULL;
	token->cmd_d->file2 = NULL;
	token->cmd_d->limiter = NULL;
	token->cmd_d->cmd = NULL;
	token->cmd_d->paths_tab = NULL;
	token->cmd_d->args = NULL;
	token->cmd_d->pid = 0;
	token->cmd_d->fd1 = -1;
	token->cmd_d->fd2 = -1;
	token->cmd_d->is_here_doc = 0;
	token->cmd_d->is_fd2_o_append = 0;
	token->cmd_d->is_in_piped = 0;
	token->cmd_d->is_out_piped = 0;
	token->cmd_d->followed_by_op = -1;
	token->cmd_d->order = 0;
	return (0);
}

int	init_token(int type, char *src, t_tokenizer_data *d)
{
	t_token	*token;

	if (!(d && d->lst))
		return (-4);
	token = (t_token *) malloc(sizeof(t_token));
	if (!token)
		return (free(src), -3);
	d->position++;
	token->position = d->position;
	token->type = type;
	token->src_len = ft_strlen(src);
	token->src = (char *) malloc((token->src_len + 1) * sizeof(char));
	if (!token->src)
		return (3);
	ft_strlcpy(token->src, src, token->src_len + 1);
	free(src);
	if (init_token_cmd_d(token) < 0)
		return (free(token->src), free(token), -3);
	if (type == D_OUT_REDI)
		token->cmd_d->is_fd2_o_append = 1;
	if (type == HEREDOC)
		token->cmd_d->is_here_doc = 1;
	return (insert_at_dll_tail(d->lst, (void *) token));
}
