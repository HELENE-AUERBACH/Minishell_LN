/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_handlers_minishell.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmorice <rmorice@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 14:28:43 by hauerbac          #+#    #+#             */
/*   Updated: 2024/09/16 18:12:29 by rmorice          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_minishell.h"

/* ************************************************************************** */
/*                           sigint_handler_in_main                           */
/* -------------------------------------------------------------------------- */
/* This function displays a newline, tells the function that we have move on  */
/* to a new line that we set as void and refreshed the display                */
/* rq : g_exit_status is set to 130                                           */
/* Input :                                                                    */
/*  - int signo : the signal used to "build" this handling function           */
/* Return :                                                                   */
/*  - None                                                                    */
/* ************************************************************************** */
void	sigint_handler_in_main(int signo)
{
	(void) signo;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_exit_status = 130;
	return ;
}

/* ************************************************************************** */
/*                           sigint_handler_in_fork                           */
/* -------------------------------------------------------------------------- */
/* This function displays a newline, tells the function that we have move on  */
/* to a new line that we set as void                                          */
/* rq : g_exit_status is set to 130                                           */
/* Input :                                                                    */
/*  - int signo : the signal used to "build" this handling function           */
/* Return :                                                                   */
/*  - None                                                                    */
/* ************************************************************************** */
void	sigint_handler_in_fork(int signo)
{
	(void) signo;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	g_exit_status = 130;
	return ;
}

/* ************************************************************************** */
/*                           sigint_handler_in_fork                           */
/* -------------------------------------------------------------------------- */
/* This function displays a newline, tells the function that we have move on  */
/* to a new line that we set as void                                          */
/* rq : g_exit_status is set to 131                                           */
/* Input :                                                                    */
/*  - int signo : the signal used to "build" this handling function           */
/* Return :                                                                   */
/*  - None                                                                    */
/* ************************************************************************** */
void	sigquit_handler_in_fork(int signo)
{
	(void) signo;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	g_exit_status = 131;
	return ;
}

/* ************************************************************************** */
/*                          heredoc_handler_in_main                           */
/* -------------------------------------------------------------------------- */
/* This function closes STDIN_FILENO. If an error occured then an error       */
/* message is display                                                         */
/* rq : g_exit_status is set to 130                                           */
/* Input :                                                                    */
/*  - int signo : the signal used to "build" this handling function           */
/* Return :                                                                   */
/*  - None                                                                    */
/* ************************************************************************** */
void	heredoc_handler_in_main(int signo)
{
	(void) signo;
	g_exit_status = 130;
	if (close(STDIN_FILENO) == -1)
		perror("STDIN_FILENO close error");
	return ;
}
