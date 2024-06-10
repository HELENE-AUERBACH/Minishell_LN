/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbocktor <jbocktor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 13:09:03 by jbocktor          #+#    #+#             */
/*   Updated: 2024/06/10 15:10:38 by jbocktor         ###   ########.fr       */
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

static long long	get_value(char *exit_value)
{
	int					sign;
	char				*endptr;
	unsigned long long	uval;

	sign = 0;
	uval = ft_strtoll(exit_value, &endptr, &sign);
	if (endptr && (endptr == exit_value || endptr[0] != '\0'))
	{
		display_err_with_prefix(exit_value, " numeric argument required\n");
		return (2);
	}
	return (uval * sign);
}

int	built_exit(t_data *d, char **args)
{
	long long	value;

	if (!args[1])
		value = d->return_code;
	else
	{
		if (too_many_args(args))
			display_err_with_prefix(args[0], " too many arguments\n");
		value = get_value(args[1]);
	}
	empty_list(&d->cmds);
	empty_list(&d->new_files);
	empty_list(&d->cmd_new_files);
	if (is_in_interactive_mode())
		rl_clear_history();
	free_data(d);
	exit(value % 256);
}
