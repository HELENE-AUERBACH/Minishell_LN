/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbocktor <jbocktor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 13:09:03 by jbocktor          #+#    #+#             */
/*   Updated: 2024/05/22 20:01:14 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	built_exit(t_data *d, char **exit_args)
{
	int	value;

	if (!exit_args[1])
		value = 0;
	else
		value = ft_atoi(exit_args[1]);
	empty_list(&d->cmds);
	empty_list(&d->new_files);
	empty_list(&d->cmd_new_files);
	if (is_in_interactive_mode())
		rl_clear_history();
	free_data(d);
	exit (value);
}
