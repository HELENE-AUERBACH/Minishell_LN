/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_cmds_3_minishell.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 17:12:13 by hauerbac          #+#    #+#             */
/*   Updated: 2024/05/29 17:07:55 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	files_open(t_token *t)
{
	if (t && t->cmd_d && t->cmd_d->file1
		&& access(t->cmd_d->file1, F_OK) == 0
		&& access(t->cmd_d->file1, R_OK) == 0)
	{
		t->cmd_d->fd1 = open(t->cmd_d->file1, O_RDONLY);
		if (t->cmd_d->is_here_doc == 0 && t->cmd_d->fd1 == -1)
			return (display_err_with_prefix(t->cmd_d->file1, \
					" input file open error\n"), -1);
		if (t->cmd_d->is_here_doc == 1 && t->cmd_d->fd1 == -1)
			return (display_error("Temporary file open error\n"), \
					-1);
	}
	if (t && t->cmd_d && t->cmd_d->is_fd2_o_append == 0 && t->cmd_d->file2)
		t->cmd_d->fd2 = open(t->cmd_d->file2, \
					O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (t && t->cmd_d && t->cmd_d->is_fd2_o_append == 1 && t->cmd_d->file2)
		t->cmd_d->fd2 = open(t->cmd_d->file2, \
					O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (t && t->cmd_d && t->cmd_d->file2 && t->cmd_d->fd2 == -1)
		return (display_err_with_prefix(t->cmd_d->file2, \
				" output file open error\n"), -2);
	return (0);
}

char	*ft_strjoin_with_free_s1(char *s1, char *s2)
{
	char	*result;
	size_t	s1_len;
	size_t	s2_len;

	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	result = (char *) malloc((s1_len + s2_len + 1) * sizeof(char));
	if (!result)
		return (NULL);
	if (s1_len == 0 && s2_len == 0)
		result[0] = '\0';
	else
	{
		if (s1_len > 0)
		{
			ft_strlcpy(result, s1, s1_len + 1);
			free(s1);
		}
		if (s2_len > 0)
			ft_strlcpy(result + s1_len, s2, s2_len + 1);
	}
	return (result);
}

static int	check_error_on_command_2(char *cmd)
{
	int	cmd_len;

	cmd_len = -1;
	if (cmd)
		cmd_len = ft_strlen(cmd);
	if (cmd && cmd_len > 0 && cmd[0] == '/' && cmd[cmd_len - 1] == '/')
		return (display_err_with_prefix(cmd, " Is a directory\n"), 126);
	if (cmd && access(cmd, F_OK) == 0 && access(cmd, X_OK) == -1)
		return (display_err_with_prefix(cmd, " Permission denied\n"), \
			126);
	if (cmd && access(cmd, F_OK) == -1)
		return (display_err_with_prefix(cmd, " command not found\n"), \
			127);
	if (cmd && cmd_len > 0 && (cmd[cmd_len - 1] == '.' \
		|| cmd[cmd_len - 1] == '/'))
		return (display_err_with_prefix(cmd, " Not a directory\n"), \
			126);
	return (perror("stat error"), EXIT_FAILURE);
}

int	check_error_on_command(char *cmd)
{
	struct stat	sb;

	if (!(cmd && stat(cmd, &sb) == 0))
		return (check_error_on_command_2(cmd));
	if ((sb.st_mode & S_IFMT) == S_IFDIR)
		return (display_err_with_prefix(cmd, " Is a directory\n"), 126);
	if (access(cmd, F_OK) == 0 && access(cmd, X_OK) == -1)
		return (display_err_with_prefix(cmd, " Permission denied\n"), \
			126);
	return (display_err_with_prefix(cmd, " No such file or directory\n"), \
		127);
}
