/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbocktor <jbocktor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 13:09:03 by jbocktor          #+#    #+#             */
/*   Updated: 2024/05/30 14:40:20 by jbocktor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	too_many_args(char **args)
{
	int	i;

	i = 0;
	while (args[i])
		i++;
	if (i > 2)
		return (1);
	return (0);
}

int	built_exit(t_data *d, char **args)
{
	char		*endptr;
	long long	value;

	if (!args[1])
		value = g_exit_status;
	else if (too_many_args(args))
		return (display_err_with_prefix(args[0], " too many arguments\n"), 1);
	else
	{
		value = ft_strtoll(args[1], &endptr);
		if (endptr == args[1] || *endptr != '\0' || value < LLONG_MIN
			|| value > LLONG_MAX)
		{
			display_err_with_prefix(args[1], " numeric argument required\n");
			value = 2;
		}
	}
	empty_list(&d->cmds);
	empty_list(&d->new_files);
	empty_list(&d->cmd_new_files);
	if (is_in_interactive_mode())
		rl_clear_history();
	free_data(d);
	exit(value % 256);
}
