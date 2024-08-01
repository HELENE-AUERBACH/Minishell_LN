/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_files_management_2_minishell.c             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 14:20:13 by hauerbac          #+#    #+#             */
/*   Updated: 2024/07/31 14:23:10 by hauerbac         ###   ########.fr       */
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
	s1.sa_handler = &heredoc_handler_in_main;
	sigemptyset(&s1.sa_mask);
	s1.sa_flags = SA_RESTART;
	sigaction(SIGINT, &s1, NULL);
}

int	write_here_doc_file(t_cmd *cmd_d)
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
			write(1, "\n", 1);
			rl_on_new_line();
			rl_replace_line("", 0);
			return (g_exit_status);
		}
		write_here_doc_file_from_a_line(cmd_d->fd1, a_line);
		free(a_line);
		a_line = "Start_Heredoc";
	}
	return (g_exit_status);
}
