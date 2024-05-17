/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbocktor <jbocktor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 13:09:00 by jbocktor          #+#    #+#             */
/*   Updated: 2024/05/17 13:33:18 by jbocktor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	there_is_an_equal(char *env_arg)
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

int	built_env(char **envp, char **env)
{
	int	i;
	int	fd;

	fd = 1;
	i = 0;
	while (env)
		i++;
	if (i > 1)
	{
		display_err_with_prefix(env[1], " No such file or directory\n");
		return (0);
	}
	i = 0;
	while (envp)
	{
		if (there_is_an_equal(envp[i]))
			write(fd, envp[i], ft_strlen(envp[i]));
		i++;
	}
	write(fd, "\n", 1);
	return (0);
}
