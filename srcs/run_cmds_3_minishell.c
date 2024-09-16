/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_cmds_3_minishell.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmorice <rmorice@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 17:12:13 by hauerbac          #+#    #+#             */
/*   Updated: 2024/09/16 18:02:09 by rmorice          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* ************************************************************************** */
/*                                 files_open                                 */
/* -------------------------------------------------------------------------- */
/* This function checks that we have the right access for the file and if we  */
/* can open them with expected options. Then, if everything is okay, it open  */
/* the files as wanted, otherwise an error message is display                 */
/* Input :                                                                    */
/*  - t_token *t      */
/* Return :                                                                   */
/*  - 0 : if everything goes well                                             */
/*  - int : the error code of the problem encounter                           */
/* ************************************************************************** */
// why do we check with access only for input ???
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

/* ************************************************************************** */
/*                          ft_strjoin_with_free_s1                           */
/* -------------------------------------------------------------------------- */
/* This function returns a string that is the result of the copy of s1 then   */
/* the copy of s2 right after the previous one. The string obtained is the    */
/* same as if we have join s1 and s2. The difference is on the fact that the  */
/* newly obtained string is return and the s1 is free during the process      */
/* Inputs :                                                                   */
/*  - char *s1 : the first string to copy                                     */
/*  - char *s2 : the second string to copy (right after the previous one)     */
/* Return :                                                                   */
/*  - char * : the string obtained (result = s1 + s2)                         */
/*  - NULL : if an error occured                                              */
/* ************************************************************************** */
// don't we need to be sure that s1 and a2 exist and aren't NULL ??? (strlen tout ça tout ça)
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

/* ************************************************************************** */
/*                          check_error_on_command_2                          */
/* -------------------------------------------------------------------------- */
/* This function checks what kind of error is encounter and displays an error */
/* message associated to it.                                                  */
/* Input :                                                                    */
/*  - char *cmd : the invalid command to check                                */
/* Return :                                                                   */
/*  - int : the error code of the problem encounter                           */
/* ************************************************************************** */
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

/* ************************************************************************** */
/*                           check_error_on_command                           */
/* -------------------------------------------------------------------------- */
/* This function checks what kind of error is encounter and displays an error */
/* message associated to it.                                                  */
/* Input :                                                                    */
/*  - char *cmd : the invalid command to check                                */
/* Return :                                                                   */
/*  - int : the error code of the problem encounter                           */
/* ************************************************************************** */
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
