/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_files_management_2_minishell.c             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 14:20:13 by hauerbac          #+#    #+#             */
/*   Updated: 2024/05/28 15:22:38 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	write_here_doc_file_from_a_line(int fd, char *a_line)
{
	ssize_t	a_line_len;

	if (a_line == NULL && g_exit_status != 130)
		write(fd, "\n", 1);
	else
	{
		a_line_len = ft_strlen(a_line);
		write(fd, a_line, a_line_len);
		write(fd, "\n", 1);
	}
}

static void	init_write_here_doc_file(void)
{
	struct sigaction	s1;

	g_exit_status = 0;
	s1.sa_handler = &heredoc_handler_in_fork;
	sigemptyset(&s1.sa_mask);
	s1.sa_flags = SA_RESTART;
	sigaction(SIGINT, &s1, NULL);
}

static void	del_el_content_without_unlink(void *content)
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
			free_cmd_d_without_unlink(token->cmd_d);
		token->cmd_d = NULL;
		free(token);
		content = NULL;
	}
	return ;
}

static void	end_write_here_doc_file(t_data *d, t_cmd *cmd_d, t_list *cur)
{
	if (cmd_d->fd1 != -1 && close(cmd_d->fd1) == -1)
		display_error("Temporary input wrotten file close error\n");
	cmd_d->fd1 = -1;
	empty_dll_before_cur(d->lst, cur, del_el_content);
	if (d->envp)
		free_tab_with_size(&d->envp, d->envp_size);
	d->envp_size = 0;
	d->nb_cmds = 0;
	dll_clear(&d->lst, del_el_content_without_unlink);
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
}

int	write_here_doc_file(t_data *d, t_cmd *cmd_d, t_list *cur)
{
	char	*a_line;
	int		limiter_len;

	init_write_here_doc_file();
	limiter_len = ft_strlen(cmd_d->limiter);
	a_line = "Start_Heredoc";
	while (a_line)
	{
		a_line = readline("> ");
		if (a_line == NULL || (a_line \
			&& ft_strncmp(cmd_d->limiter, a_line, limiter_len) == 0
				&& a_line[limiter_len] == '\0'))
		{
			end_write_here_doc_file(d, cmd_d, cur);
			exit (g_exit_status);
		}
		write_here_doc_file_from_a_line(cmd_d->fd1, a_line);
		free(a_line);
		a_line = "Start_Heredoc";
	}
	return (g_exit_status);
}
