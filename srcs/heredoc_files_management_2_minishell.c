/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_files_management_2_minishell.c             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 14:20:13 by hauerbac          #+#    #+#             */
/*   Updated: 2024/05/15 17:08:21 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_minishell.h"

static void	free_p(char **ptr)
{
	free(*ptr);
	*ptr = NULL;
}

static void	reset_buffer(char *buff, ssize_t i)
{
	while (i < HEREDOC_BUFFER_SIZE + 1)
		buff[i++] = '\0';
}

int	has_got_delimiter(char *buff, char *limiter, int limiter_len)
{
	ssize_t		buff_len;

	if (g_exit_status == 130)
		return (1);
	if (buff == NULL)
		return (0);
	buff_len = ft_strlen(buff);
	if (buff_len == limiter_len + 1
		&& ft_strncmp(limiter, buff, limiter_len) == 0
		&& buff[limiter_len] == '\n')
		return (1);
	return (0);
}

static int	write_here_doc_file_from_buffer(int fd, char *b, char *limiter,
			int limiter_len)
{
	ssize_t	read_size;

	read_size = -1;
	while (read_size != 0 && g_exit_status != 130)
	{
		reset_buffer(b, 0);
		read_size = read(0, b, HEREDOC_BUFFER_SIZE);
		if (g_exit_status == 130)
		{
			read_size = 0;
			return (g_exit_status);
		}
		if (read_size < 0)
			return (display_error("read error for buffer\n"), -1);
		if (read_size > 0 && has_got_delimiter(b, limiter, limiter_len))
			read_size = read_size - limiter_len - 1;
		if (read_size > 0)
		{
			reset_buffer(b, read_size);
			write(fd, b, read_size);
		}
	}
	return (0);
}

int	write_here_doc_file(t_cmd *cmd_d)
{
	char				*b;
	int					result;
	int					limiter_len;
	struct sigaction	s1;

	g_exit_status = 0;
	s1.sa_handler = &heredoc_handler_in_fork;
	sigemptyset(&s1.sa_mask);
	s1.sa_flags = SA_RESTART;
	sigaction(SIGINT, &s1, NULL);
	b = (char *) malloc(sizeof(char) * (HEREDOC_BUFFER_SIZE + 1));
	if (!b)
		return (display_error("Malloc error for buffer\n"), -1);
	limiter_len = ft_strlen(cmd_d->limiter);
	result = write_here_doc_file_from_buffer(cmd_d->fd1, b, cmd_d->limiter,
			limiter_len);
	free_p(&b);
	if (g_exit_status == 0)
	{
		kill(0, SIGINT);
		g_exit_status = 0;
		result = 0;
	}
	return (result);
}
