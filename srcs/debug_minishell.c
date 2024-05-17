/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_minishell.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 17:25:32 by hauerbac          #+#    #+#             */
/*   Updated: 2024/05/06 17:21:54 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_token_cmd_d_args(int fd, t_cmd *cmd_d)
{
	int	i;

	if (cmd_d->args)
	{
		ft_putstr_fd(", args : [", fd);
		i = 0;
		while (cmd_d->args[i])
		{
			if (i > 0)
				ft_putstr_fd(", ", fd);
			ft_putstr_fd(cmd_d->args[i], fd);
			i++;
		}
		ft_putstr_fd("]", fd);
	}
}

static void	print_token_cmd_d_paths_tab(int fd, t_cmd *cmd_d)
{
	int	i;

	if (cmd_d->paths_tab)
	{
		ft_putstr_fd(", paths_tab : [", fd);
		i = 0;
		while (cmd_d->paths_tab[i])
		{
			if (i > 0)
				ft_putstr_fd(", ", fd);
			ft_putstr_fd(cmd_d->paths_tab[i], fd);
			i++;
		}
		ft_putstr_fd("]", fd);
	}
}

static char	*get_token_type_into_enum_value(int type)
{
	char	**tab;

	if (type < 0 || type > 17)
		return ("NO_VALUE");
	tab = (char *[18]){"UNKNOWN", "S_IN_REDI", "S_OUT_REDI", "HEREDOC", \
		"D_OUT_REDI", "COMMAND", "BI", "PIPE", "SEMICOLON", \
		"AMPERSAND", "DOUBLE_SEMICOLON", "LOGICAL_AND", "LOGICAL_OR", \
		"SEMICOLON_AMPERSAND", "DOUBLE_SEMICOLON_AMPERSAND", \
		"PIPE_AMPERSAND", "OPENING_PARENTHESIS", "CLOSING_PARENTHESIS"};
	return (tab[type]);
}

static void	print_token_cmd_d(int fd, t_cmd *cmd_d)
{
	ft_putstr_fd(", file1 : ", fd);
	ft_putstr_fd(cmd_d->file1, fd);
	ft_putstr_fd(", file2 : ", fd);
	ft_putstr_fd(cmd_d->file2, fd);
	ft_putstr_fd(", limiter : ", fd);
	ft_putstr_fd(cmd_d->limiter, fd);
	ft_putstr_fd(", cmd : ", fd);
	ft_putstr_fd(cmd_d->cmd, fd);
	ft_putstr_fd(", fd1 : ", fd);
	ft_putnbr_fd(cmd_d->fd1, fd);
	ft_putstr_fd(", fd2 : ", fd);
	ft_putnbr_fd(cmd_d->fd2, fd);
	ft_putstr_fd(", is_here_doc : ", fd);
	ft_putnbr_fd(cmd_d->is_here_doc, fd);
	ft_putstr_fd(", is_in_piped: ", fd);
	ft_putnbr_fd(cmd_d->is_in_piped, fd);
	ft_putstr_fd(", is_out_piped: ", fd);
	ft_putnbr_fd(cmd_d->is_out_piped, fd);
	ft_putstr_fd(", followed_by_op: ", fd);
	ft_putstr_fd(get_token_type_into_enum_value(cmd_d->followed_by_op), fd);
	ft_putstr_fd(", order : ", fd);
	ft_putnbr_fd(cmd_d->order, fd);
	print_token_cmd_d_args(fd, cmd_d);
	print_token_cmd_d_paths_tab(fd, cmd_d);
}

void	print_dll_el_content(int fd, void *el_content)
{
	t_token	*token;

	token = (t_token *) el_content;
	ft_putstr_fd("Position : ", fd);
	ft_putnbr_fd(token->position, fd);
	ft_putstr_fd(", type : ", fd);
	ft_putstr_fd(get_token_type_into_enum_value(token->type), fd);
	ft_putstr_fd(", src_len : ", fd);
	ft_putnbr_fd(token->src_len, fd);
	ft_putstr_fd(", src : ", fd);
	ft_putstr_fd(token->src, fd);
	if (token->cmd_d)
		print_token_cmd_d(fd, token->cmd_d);
	return ;
}
