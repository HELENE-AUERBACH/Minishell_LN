/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_echo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbocktor <jbocktor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 13:08:56 by jbocktor          #+#    #+#             */
/*   Updated: 2024/05/17 14:50:15 by jbocktor         ###   ########.fr       */
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

int	built_echo(char **echo)
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
		write(1, echo_string[i], ft_strlen(echo_string[i]));
		i++;
		if (echo_string[i] != NULL)
			write(1, " ", 1);
	}
	if (new_line)
		write(1, "\n", 1);
	return (0);
}
