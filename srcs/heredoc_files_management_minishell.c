/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_files_management_minishell.c               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 13:49:57 by hauerbac          #+#    #+#             */
/*   Updated: 2024/05/20 16:53:05 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_minishell.h"

static int	write_here_doc_file_in_fork(t_cmd *cmd_d)
{
	pid_t				pid;
	int					wstatus;

	pid = fork();
	if (pid == -1)
		return (display_error("Fork error"), -1);
	if (pid == 0)
		return (write_here_doc_file(cmd_d));
	else
	{
		if (waitpid(pid, &wstatus, 0) <= 0)
			return (display_error("waitpid failed"), -2);
		write(1, "\n", 1);
		if (WIFEXITED(wstatus) && WEXITSTATUS(wstatus) < 0)
			return (WEXITSTATUS(wstatus));
		return (g_exit_status);
	}
}

static char	*get_file_name_for_heredoc(char *limiter, t_dll_el *prev)
{
	char	*index;
	size_t	index_len;
	char	*file_name;

	index = NULL;
	if (prev && prev->content)
		index = ft_itoa(((t_token *)(prev->content))->cmd_d->fd1);
	if (!index)
	{
		index = (char *) malloc((1 + 1) * sizeof(char));
		if (!index)
			return (NULL);
		ft_strlcpy(index, "0", 1 + 1);
	}
	index_len = ft_strlen(index);
	file_name = (char *) malloc((15 + ft_strlen(limiter) + index_len + 2)
			* sizeof(char));
	if (!file_name)
		return (free(index), NULL);
	ft_strlcpy(file_name, ".here_doc_file_", 15 + 1);
	ft_strlcpy(file_name + 15, limiter, ft_strlen(limiter) + 1);
	ft_strlcpy(file_name + 15 + ft_strlen(limiter), "_", 1 + 1);
	file_name = ft_strjoin_with_free(file_name, index);
	return (file_name);
}

int	create_heredoc_file(t_token *t, t_dll_el *prev)
{
	t->cmd_d->limiter = (char *) malloc((t->src_len + 1) * sizeof(char));
	if (!t->cmd_d->limiter)
		return (-3);
	ft_strlcpy(t->cmd_d->limiter, t->src, t->src_len + 1);
	t->cmd_d->is_here_doc = 1;
	t->cmd_d->file1 = get_file_name_for_heredoc(t->cmd_d->limiter, prev);
	if (!t->cmd_d->file1)
		return (free_cmd_d(t->cmd_d), -3);
	t->cmd_d->fd1 = open(t->cmd_d->file1, O_WRONLY | O_CREAT | O_TRUNC,
			0644);
	if (t->cmd_d->fd1 == -1)
		return (display_err_with_prefix(t->cmd_d->file1, \
				" temporary file creation error\n"), -4);
	if (write_here_doc_file_in_fork(t->cmd_d) < 0)
	{
		close(t->cmd_d->fd1);
		t->cmd_d->fd1 = -1;
		return (display_err_with_prefix(t->cmd_d->file1, \
				" temporary file writing error\n"), -5);
	}
	/*if (close(t->cmd_d->fd1) == -1)
		return (display_err_with_prefix(t->cmd_d->file1, \
				" temporary file close error\n"), -6);
	t->cmd_d->fd1 = -1;*/
	return (0);
}

void	close_in_file_and_free_file_name(t_cmd *cmd_d)
{
	if (cmd_d)
	{
		if (cmd_d->is_here_doc && cmd_d->fd1 != -1
			&& close(cmd_d->fd1) == -1)
		{
			perror("Temporary input file close error");
		}
		else if (!cmd_d->is_here_doc && cmd_d->fd1 != -1
			&& close(cmd_d->fd1) == -1)
		{
			perror("Input file close error");
			write(2, cmd_d->file1, ft_strlen(cmd_d->file1));
			write(2, "\n", 1);
		}
		else
			cmd_d->fd1 = -1;
		if (cmd_d->is_here_doc && cmd_d->limiter && cmd_d->file1
			&& access(cmd_d->file1, F_OK) == 0
			&& access(cmd_d->file1, R_OK) == 0
			&& unlink(cmd_d->file1) != 0)
			perror("unlink() error");
		if (cmd_d->file1)
			free(cmd_d->file1);
		cmd_d->file1 = NULL;
	}
}

void	close_out_file_and_free_file_name(t_cmd *cmd_d)
{
	if (cmd_d)
	{
		if (cmd_d->fd2 != -1 && close(cmd_d->fd2) == -1)
		{
			perror("Output file close error");
			write(2, cmd_d->file2, ft_strlen(cmd_d->file2));
			write(2, "\n", 1);
		}
		else
			cmd_d->fd2 = -1;
		if (cmd_d->file2)
			free(cmd_d->file2);
		cmd_d->file2 = NULL;
	}
}
