/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbocktor <jbocktor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 13:09:00 by jbocktor          #+#    #+#             */
/*   Updated: 2024/06/12 11:53:42 by jbocktor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	there_is_an_equal(char *env_arg)
{
	int	i;

	i = 0;
	while (env_arg[i])
	{
		if (env_arg[i] == '=')
			return (1);
		i++;
	}
	return (0);
}

int	built_env(char **envp, char **env, int fd)
{
	int	i;

	i = 0;
	while (env[i])
		i++;
	if (i > 1)
	{
		if (env[1][0] == '-')
		{
			return (display_err_with_2_prefixes(env[0], env[1],
					" invalid option\n"), 1);
		}
		return (display_err_with_prefix(env[1], " invalid argument\n"), 1);
	}
	i = 0;
	while (envp[i])
	{
		if (there_is_an_equal(envp[i]))
		{
			write(fd, envp[i], ft_strlen(envp[i]));
			write(fd, "\n", 1);
		}
		i++;
	}
	return (0);
}
