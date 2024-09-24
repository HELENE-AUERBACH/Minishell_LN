/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_files_management_minishell.c               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmorice <rmorice@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 13:49:57 by hauerbac          #+#    #+#             */
/*   Updated: 2024/09/24 13:45:41 by rmorice          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* ************************************************************************** */
/*                        open_and_write_here_doc_file                        */
/* -------------------------------------------------------------------------- */
/* This function opens the file associated to the pseudo-heredoc and write    */
/* into it everything that is taped into the prompt until the specified       */
/* limiter. Then the temporary file is closed.                                */
/* rq : The actions associated to SIGINT and SIGQUIT are changed              */
/* Input :                                                                    */
/*  - t_token *t : a structure that contained datas about the current token   */
/* Return :                                                                   */
/*  - 0 (res) : if write stop by LIMITER and not by SIGINT / SIGQUIT (signal) */
/*  - int : the value associated to the signal exit status (g_exit_status)    */
/* ************************************************************************** */
static int	open_and_write_here_doc_file(t_token *t)
{
	int	fd0;
	int	res;

	fd0 = dup(STDIN_FILENO);
	if (fd0 == -1)
		return (display_error("STDIN_FILENO dup failed"), -8);
	t->cmd_d->fd1 = open(t->cmd_d->file1, O_WRONLY | O_CREAT | O_TRUNC, \
				0600);
	if (t->cmd_d->fd1 == -1)
		return (display_error("Temporary file creation error\n"), -4);
	res = write_here_doc_file(t->cmd_d);
	if (t->cmd_d->fd1 != -1 && close(t->cmd_d->fd1) == -1)
		return (display_error("Temporary file close error\n"), -6);
	t->cmd_d->fd1 = -1;
	set_signals_actions();
	if (dup2(fd0, STDIN_FILENO) == -1)
		return (display_error("STDIN_FILENO dup2 failed"), -7);
	if (close(fd0) != 0)
		return (display_error("STDIN_FILENO dup des. close error"), -9);
	if (g_exit_status == 0)
		return (res);
	return (g_exit_status);
}

/* ************************************************************************** */
/*                         get_file_name_for_heredoc                          */
/* -------------------------------------------------------------------------- */
/* This function generates the name to associated to the temporary file.      */
/* It follow the model : ".here_doc_file_"<limiter>"_"<index>                 */
/* Inputs :                                                                   */
/*  - char *limiter : the limiter of the "heredoc"                            */
/*  - t_dll_el *prev : previous element of double list relatives to cmd datas */
/* Return :                                                                   */
/*  - 0 : if everything goes well                                             */
/*  - int : the error code of the problem encounter                           */
/* ************************************************************************** */
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

/* ************************************************************************** */
/*                            create_heredoc_file                             */
/* -------------------------------------------------------------------------- */
/* This function saves the limiter and specified that we have a "heredoc".    */
/* The filename of the temporary file use for the heredoc is determined, then */
/* it's open and we write in it what was tape by the user (until limiter)     */
/* Inputs :                                                                   */
/*  - t_token *t : a structure that contained datas about the current token   */
/*  - t_dll_el *current : an elt of a double list relative to cmd datas       */
/* Return :                                                                   */
/*  - 0 : if everything goes well                                             */
/*  - int : the error code of the problem encounter                           */
/* ************************************************************************** */
int	create_heredoc_file(t_token *t, t_dll_el *current)
{
	t->cmd_d->limiter = (char *) malloc((t->src_len + 1) * sizeof(char));
	if (!t->cmd_d->limiter)
		return (-3);
	ft_strlcpy(t->cmd_d->limiter, t->src, t->src_len + 1);
	t->cmd_d->is_here_doc = 1;
	t->cmd_d->file1 = get_file_name_for_heredoc(t->cmd_d->limiter, \
							current->prev);
	if (!t->cmd_d->file1)
		return (free_cmd_d(&t->cmd_d), -3);
	if (open_and_write_here_doc_file(t) < 0)
		return (display_error("Temporary file writing error\n"), -5);
	return (g_exit_status);
}

/* ************************************************************************** */
/*                      close_in_file_and_free_file_name                      */
/* -------------------------------------------------------------------------- */
/* This function closes the infile (temporary file if heredoc) and frees the  */
/* string that contained the file name                                        */
/* In the case of a "heredoc", the file is unlink (disappear after use)       */
/* Input :                                                                    */
/*  - t_cmd *cmd_d : a struct that contained datas relative to a specific cmd */
/* Return :                                                                   */
/*  - None                                                                    */
/* ************************************************************************** */
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

/* ************************************************************************** */
/*                     close_out_file_and_free_file_name                      */
/* -------------------------------------------------------------------------- */
/* This function closes the output file opened, frees the variable associated */
/* to this file name and makes it point toward NULL. The fd2 (ouput file      */
/* descriptor) variable is set to -1                                          */
/* If an error occured then an error message is displayed.                    */
/* Input :                                                                    */
/*  - t_cmd *cmd_d : a struct that contained datas relative to a specific cmd */
/* Return :                                                                   */
/*  - None                                                                    */
/* ************************************************************************** */
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
