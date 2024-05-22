/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_echo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbocktor <jbocktor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 13:08:56 by jbocktor          #+#    #+#             */
/*   Updated: 2024/05/20 18:14:45 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	start_n(const char *arg)
{
	size_t	i;

	i = 2;
	if (ft_strncmp(arg, "-n", ft_strlen("-n")) != 0)
		return (0);
	while (i < ft_strlen(arg))
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

int	built_echo(char **echo, int fd)
{
	char	**echo_string;
	int		i;
	int		new_line;

	echo_string = &echo[1];
	new_line = 1;
	i = 0;
	while (echo_string[i] != NULL && start_n(echo_string[i]))
	{
		new_line = 0;
		i++;
	}
	while (echo_string[i] != NULL)
	{
		while (echo_string[i] == NULL)
			i++;
		write(fd, echo_string[i], ft_strlen(echo_string[i]));
		i++;
		if (echo_string[i] != NULL)
			write(fd, " ", 1);
	}
	if (new_line)
		write(fd, "\n", 1);
	return (0);
}
