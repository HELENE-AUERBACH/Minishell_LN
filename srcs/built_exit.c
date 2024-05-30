/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbocktor <jbocktor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 13:09:03 by jbocktor          #+#    #+#             */
/*   Updated: 2024/05/30 13:23:28 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	built_exit(t_data *d, char **exit_args)
{
	char		*endptr;
	long long	value;

	if (!exit_args[1])
		value = g_exit_status;
	else
	{
		value = ft_strtoll(exit_args[1], &endptr);
		if (endptr == exit_args[1] || *endptr != '\0'
			|| value < LLONG_MIN || value > LLONG_MAX)
		{
			display_err_with_prefix(exit_args[1], \
						" numeric argument required\n");
			value = 2;
		}
	}
	empty_list(&d->cmds);
	empty_list(&d->new_files);
	empty_list(&d->cmd_new_files);
	if (is_in_interactive_mode())
		rl_clear_history();
	free_data(d);
	exit (value % 256);
}
