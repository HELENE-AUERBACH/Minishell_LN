/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_echo_minishell.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 11:32:57 by hauerbac          #+#    #+#             */
/*   Updated: 2024/06/25 13:27:27 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	check_options_and_newline(int *i, int *with_a_newline,
			char **args)
{
	size_t	j;

	while (args[*i] && ft_strlen(args[*i]) > 0 \
			&& ft_strncmp("-n", args[*i], 2) == 0)
	{
		*with_a_newline = 0;
		j = 2;
		while (j < ft_strlen(args[*i]))
		{
			if (args[*i][j] != 'n')
			{
				*with_a_newline = 1;
				return ;
			}
			j++;
		}
		(*i)++;
	}
}

int	built_echo(char **args, int fd)
{
	int	i;
	int	with_a_newline;

	i = 1;
	with_a_newline = 1;
	check_options_and_newline(&i, &with_a_newline, args);
	while (args[i] && ft_strlen(args[i]) > 0)
	{
		write(fd, args[i], ft_strlen(args[i]));
		i++;
		if (args[i] && ft_strlen(args[i]) > 0)
			write(fd, " ", 1);
	}
	if (with_a_newline)
		write(fd, "\n", 1);
	return (0);
}
