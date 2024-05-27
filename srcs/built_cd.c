/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_cd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbocktor <jbocktor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 13:08:44 by jbocktor          #+#    #+#             */
/*   Updated: 2024/05/27 14:26:22 by jbocktor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int	update_pwd_and_old(char ***envp, int *envp_size, char *new_pwd)
{
	char	*value;
	char	**envi;
	int		i;

	i = 0;
	value = NULL;
	envi = (*envp);
	if (get_value_from_envp(&value, "PWD", *envp_size, *envp) != 0)
		return (3);
	while (envi[i])
	{
		if (ft_strncmp(envi[i], "OLDPWD", 6) == 0)
		{
			free(envi[i]);
			envi[i] = ft_strjoin("OLDPWD=", value);
			if (envi[i] == NULL)
				return (3);
			break ;
		}
		i++;
	}
	while (envi[i])
	{
		if (ft_strncmp(envi[i], "PWD", 3) == 0)
		{
			free(envi[i]);
			envi[i] = ft_strjoin("PWD=", new_pwd);
			if (envi[i] == NULL)
				return (3);
			break ;
		}
		i++;
	}
	return (0);
}

int	built_cd(char ***envp, int *envp_size, char **cd)
{
	int		i;
	char	*value;

	value = NULL;
	i = 0;
	while (cd[i])
		i++;
	if (i > 2)
		return (0);
	// "bash: %s: too many arguments\n";
	if (i == 1 || ft_strncmp(cd[1], "~", 1) == 0)
	{
		if (get_value_from_envp(&value, "HOME", *envp_size, *envp) != 0)
			return (3);
	}
	else
		value = cd[1];
	if (chdir(value) < 0)
	{
		write(1, "salut", 5);
		// "bash: %s: too many arguments\n";
		return (-1);
	}
	else
		return (update_pwd_and_old(envp, envp_size, value));
	free(value);
}
