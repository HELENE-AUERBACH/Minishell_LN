/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_minishell.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmorice <rmorice@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 14:07:09 by hauerbac          #+#    #+#             */
/*   Updated: 2024/09/24 13:53:25 by rmorice          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_minishell.h"

/* ************************************************************************** */
/*                        init_utils_data_and_new_src                         */
/* -------------------------------------------------------------------------- */
/* This function initialises utils_data and makes new_src point to null       */
/* Inputs :                                                                   */
/*  - int *utils_data : array of datas about internal criteria of token to be */
/*  - char **new_src : a pointer to a str that contained the update cmd line  */
/*  - int str_len : the length of the full command line                       */
/* Return :                                                                   */
/*  - NULL                                                                    */
/* ************************************************************************** */
void	init_utils_data_and_new_src(int *utils_data, char **new_src,
		int str_len)
{
	init_utils_data(utils_data, str_len);
	*new_src = NULL;
}

/* ************************************************************************** */
/*                  get_end_index_of_file_or_delimiter_name                   */
/* -------------------------------------------------------------------------- */
/* This function traverses the string starting from the index i until it met  */
/* a metacharacter. Then it returns the index of the first metacharacter      */
/* encounter.                                                                 */
/* Inputs :                                                                   */
/*  - int *i : a pointer to the index where we start                          */
/*  - const char *str : the command line to use as reference                  */
/* Return :                                                                   */
/*  - int : index of the first metacharacter encounter                        */
/* ************************************************************************** */
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

/* ************************************************************************** */
/*                              init_token_cmd_d                              */
/* -------------------------------------------------------------------------- */
/* This function initialises the structure t_token token received in argument */
/* This structure contained all the datas relative to the current command     */
/* Input :                                                                    */
/*  - t_tokenizer *token : the structure to initialise                        */
/* Return :                                                                   */
/*  - 0 : if everything goes well                                             */
/*  - -3 : otherwise (malloc error)                                           */
/* ************************************************************************** */
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

/* ************************************************************************** */
/*                                 init_token                                 */
/* -------------------------------------------------------------------------- */
/* This function initialises the structure t_tokenizer_data d received in     */
/* arguments. This structure describes the current state of the analyse       */
/* Inputs :                                                                   */
/*  - int type : an int that defines the kind of token met                    */
/*  - const char *str : the command line to use as reference                  */
/*  - t_tokenizer_data *d : the structure to initialise                       */
/* Return :                                                                   */
/*  - 0 : if everything goes well                                             */
/*  - int : otherwise                                                         */
/* ************************************************************************** */
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
