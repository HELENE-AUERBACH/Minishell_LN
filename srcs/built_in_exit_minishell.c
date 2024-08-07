/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_exit_minishell.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 13:34:39 by hauerbac          #+#    #+#             */
/*   Updated: 2024/06/25 16:29:06 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	too_many_args(char **args)
{
	int	i;

	i = 1;
	while (args[i])
	{
		i++;
	}
	if (i > 2)
		return (1);
	return (0);
}

static long long	get_value(int *error, char *exit_value)
{
	int					sign;
	char				*endptr;
	unsigned long long	uval;

	sign = 0;
	uval = ft_strtoll(exit_value, &endptr, &sign);
	if (endptr && (endptr == exit_value || endptr[0] != '\0'))
	{
		display_err_with_prefix(exit_value, \
					" numeric argument required\n");
		*error = 1;
		return (2);
	}
	return (uval * sign);
}

int	built_exit(t_data *d, char **args)
{
	long long	value;
	int			error;

	if (!args[1])
		value = d->return_code;
	else
	{
		error = 0;
		value = get_value(&error, args[1]);
		if (!error && too_many_args(args))
			return (display_err_with_prefix(args[0], \
						" too many arguments\n"), 1);
	}
	empty_list(&d->cmds);
	empty_list(&d->new_files);
	empty_list(&d->cmd_new_files);
	if (is_in_interactive_mode())
		rl_clear_history();
	free_data(d);
	exit (value % 256);
}
