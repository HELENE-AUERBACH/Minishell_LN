/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_files_management_minishell.c               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 13:49:57 by hauerbac          #+#    #+#             */
/*   Updated: 2024/06/04 12:26:58 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	open_and_write_here_doc_file_in_fork(t_data *d, t_token *t,
			t_list *cur)
{
	pid_t	pid;
	int		wstatus;
	int		res;

	pid = fork();
	if (pid == -1)
		return (display_error("Fork error"), -1);
	if (pid == 0)
	{
		t->cmd_d->fd1 = open(t->cmd_d->file1, \
			O_WRONLY | O_CREAT | O_TRUNC, 0600);
		if (t->cmd_d->fd1 == -1)
			return (display_error("T.f.cre.e\n"), free_data(d), -4);
		res = write_here_doc_file(d, t->cmd_d, cur);
		if (t->cmd_d->fd1 != -1 && close(t->cmd_d->fd1) == -1)
			return (display_error("T.f.clo.e\n"), free_data(d), -6);
		t->cmd_d->fd1 = -1;
		return (ft_lstclear(&d->cmds, del_el_content), free_data(d), \
			res);
	}
	if (waitpid(pid, &wstatus, 0) <= 0)
		return (display_error("waitpid failed"), -2);
	if (g_exit_status == 0 && WIFEXITED(wstatus))
		return (WEXITSTATUS(wstatus));
	return (g_exit_status);
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

int	create_heredoc_file(t_data *d, t_token *t, t_dll_el *current)
{
	t_list	*curr;

	t->cmd_d->limiter = (char *) malloc((t->src_len + 1) * sizeof(char));
	if (!t->cmd_d->limiter)
		return (-3);
	ft_strlcpy(t->cmd_d->limiter, t->src, t->src_len + 1);
	t->cmd_d->is_here_doc = 1;
	t->cmd_d->file1 = get_file_name_for_heredoc(t->cmd_d->limiter, \
							current->prev);
	if (!t->cmd_d->file1)
		return (free_cmd_d(&t->cmd_d), -3);
	curr = (t_list *) current;
	if (open_and_write_here_doc_file_in_fork(d, t, curr) < 0)
	{
		close(t->cmd_d->fd1);
		t->cmd_d->fd1 = -1;
		return (display_error("Temporary file writing error\n"), -5);
	}
	if (t->cmd_d->fd1 != -1 && close(t->cmd_d->fd1) == -1)
		return (perror("Temporary input just created file close error"),
			-6);
	return (g_exit_status);
}

void	close_in_file_and_free_file_name(t_cmd *cmd_d)
{
	if (cmd_d)
	{
		if (cmd_d->is_here_doc && cmd_d->fd1 != -1
			&& close(cmd_d->fd1) == -1)
			perror("Temporary input file close error");
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
			&& unlink(cmd_d->file1) != 0 && errno != 2)
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
