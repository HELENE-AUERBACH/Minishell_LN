/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_files_management_2_minishell.c             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmorice <rmorice@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 14:20:13 by hauerbac          #+#    #+#             */
/*   Updated: 2024/09/23 13:35:59 by rmorice          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* ************************************************************************** */
/*                      write_here_doc_file_from_a_line                       */
/* -------------------------------------------------------------------------- */
/* This function writes a_line followed by a newline into fd1.                */
/* rq : if a_line is NULL and g_exit_status != 130 then a newline is write    */
/* into fd1                                                                   */
/* Inputs :                                                                   */
/*  - int fd : the file descriptor where we should write                      */
/*  - char *a_line : the string to write, followed by a newline               */
/* Return :                                                                   */
/*  - None                                                                    */
/* ************************************************************************** */
// hmm... if a_line == NULL && g_exit_status == 130
// -> else -> ft_strlen => SegFault ???
// Am I wrong ? Or is it something that can never happened for sure ???
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

/* ************************************************************************** */
/*                          init_write_here_doc_file                          */
/* -------------------------------------------------------------------------- */
/* This function changes the action associated to signal SIGINT to the one    */
/* associated to s1                                                           */
/* To do so it specified that s1.sa_handler should closes STDIN_FILENO and    */
/* g_exit_status is set to 130                                                */
/* rq : every signals given by s1.sa_mask is emptied before sigaction call    */
/* Input :                                                                    */
/*  - None                                                                    */
/* Return :                                                                   */
/*  - None                                                                    */
/* ************************************************************************** */
static void	init_write_here_doc_file(void)
{
	struct sigaction	s1;

	g_exit_status = 0;
	s1.sa_handler = &heredoc_handler_in_main;
	sigemptyset(&s1.sa_mask);
	s1.sa_flags = SA_RESTART;
	sigaction(SIGINT, &s1, NULL);
}

/* ************************************************************************** */
/*                            write_here_doc_file                             */
/* -------------------------------------------------------------------------- */
/* This function changes the action associated to SIGINT and reads everything */
/* that is taped into the prompt until the specified limiter.                 */
/* Everything that is tape is write into fd1 file (temporary file associated  */
/* to heredoc).                                                               */
/* rq : once the limiter is encountered then we add a newline into fd1 file   */
/* and we tells the function that we have move on to a new line that we set   */
/* as void                                                                    */
/* Input :                                                                    */
/*  - t_cmd *cmd_d : a struct that contained datas relative to a specific cmd */
/* Return :                                                                   */
/*  - g_exit_status (unmodified in this function)                             */
/* ************************************************************************** */
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
