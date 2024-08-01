/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_handlers_minishell.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 14:28:43 by hauerbac          #+#    #+#             */
/*   Updated: 2024/07/31 14:53:17 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_minishell.h"

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

void	sigint_handler_in_fork(int signo)
{
	(void) signo;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	g_exit_status = 130;
	return ;
}

void	sigquit_handler_in_fork(int signo)
{
	(void) signo;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	g_exit_status = 131;
	return ;
}

void	heredoc_handler_in_main(int signo)
{
	(void) signo;
	g_exit_status = 130;
	if (close(STDIN_FILENO) == -1)
		perror("STDIN_FILENO close error");
	return ;
}
