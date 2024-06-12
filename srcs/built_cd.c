/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_cd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbocktor <jbocktor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 13:08:44 by jbocktor          #+#    #+#             */
/*   Updated: 2024/06/12 11:20:51 by jbocktor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	update_pwd_and_old(char ***envp, int *envp_size, char *new_pwd,
		char *old_pwd)
{
	char	*value;
	int		i;

	i = 0;
	value = NULL;
	if (new_pwd)
		free(new_pwd);
	new_pwd = getcwd(NULL, 0);
	if (modify_pwd_value(envp_size, envp, old_pwd, "OLDPWD") == -3)
		return (3);
	if (modify_pwd_value(envp_size, envp, new_pwd, "PWD") == -3)
		return (3);
	if (old_pwd)
		free(old_pwd);
	if (new_pwd)
		free(new_pwd);
	return (0);
}

static int	get_chdir_value(char ***envp, int *envp_size, char **cd,
		char **value)
{
	int	i;

	i = 0;
	while (cd[i])
		i++;
	if (i > 2)
		display_err_with_prefix(cd[0], " too many arguments\n");
	else if (i == 1 || ft_strncmp(cd[1], "~", 1) == 0)
	{
		get_value_from_envp(value, "HOME", *envp_size, *envp);
		if (!(*value))
		{
			display_err_with_prefix(cd[0], " HOME not set\n");
			return (-1);
		}
	}
	else if (i == 2)
	{
		(*value) = ft_strdup(cd[1]);
		if (!(*value))
			return (-3);
	}
	return (0);
}

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

int	built_cd(char ***envp, int *envp_size, char **cd)
{
	char	*value;
	int		return_value;
	char	*old_value;

	return_value = get_chdir_value(envp, envp_size, cd, &value);
	if (return_value < 0)
		return (return_value * -1);
	old_value = getcwd(NULL, 0);
	if (!old_value)
		return (display_err_with_2_prefixes(cd[0], cd[1], \
				" No such file or directory\n"), 1);
	if (chdir(value) < 0)
	{
		if (old_value)
			free(old_value);
		if (value)
			free(value);
		if (too_many_args(cd) == 0 && cd[1])
			display_err_with_2_prefixes(cd[0], cd[1], \
				" No such file or directory\n");
		return (1);
	}
	else
		return (update_pwd_and_old(envp, envp_size, value, old_value));
}
