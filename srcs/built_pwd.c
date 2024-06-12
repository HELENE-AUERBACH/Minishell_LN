/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_pwd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbocktor <jbocktor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 13:09:10 by jbocktor          #+#    #+#             */
/*   Updated: 2024/06/12 11:21:06 by jbocktor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	no_option(char **pwd)
{
	int	i;

	i = 0;
	while (pwd[i])
	{
		if (pwd[i][0] == '-')
		{
			if (ft_strncmp("-", pwd[i], ft_strlen(pwd[i])) != 0)
			{
				display_err_with_2_prefixes(pwd[0], pwd[i], \
					" invalid option\n");
				return (-1);
			}
		}
		i++;
	}
	return (0);
}

int	built_pwd(char **pwd)
{
	char	*path;
	int		i;

	i = 0;
	if (no_option(pwd) == -1)
		return (1);
	path = getcwd(NULL, 0);
	if (!path)
	{
		display_err_with_2_prefixes(pwd[0], \
			" error retrieving current directory", \
			" getcwd: cannot access parent directories\n");
		return (1);
	}
	write(1, path, ft_strlen(path));
	write(1, "\n", 1);
	free(path);
	return (0);
}
