/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_cd_minishell.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 16:57:29 by hauerbac          #+#    #+#             */
/*   Updated: 2024/07/03 17:02:28 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	update_oldpwd_pwd_vars_in_envp(char ***envp, int *envp_size,
			char **previous_pwd)
{
	int		result;
	char	*new_dir_val;

	result = updenv(envp, envp_size, "OLDPWD=", *previous_pwd);
	if (result == 0)
	{
		new_dir_val = getcwd(NULL, 0);
		if (!new_dir_val)
		{
			display_err_with_prefix("cd", \
				" error retrieving current directory\n");
			result = 1;
		}
		else
		{
			result = updenv(envp, envp_size, "PWD=", new_dir_val);
			free(new_dir_val);
		}
	}
	if (previous_pwd && *previous_pwd)
		free(*previous_pwd);
	*previous_pwd = NULL;
	return (result);
}

static int	change_directory(char *new_dir, char **envp, int envp_size)
{
	char	*new_dir_val;

	if (get_value_from_envp(&new_dir_val, new_dir, envp_size, envp) == 3)
		return (-3);
	if (!new_dir_val)
	{
		write(2, "cd: ", 4);
		write(2, new_dir, ft_strlen(new_dir));
		write(2, " not set\n", 9);
		return (-1);
	}
	else if (chdir(new_dir_val) == -1)
		return (write(2, "cd: ", 4), perror(new_dir_val), \
			free(new_dir_val), -2);
	return (free(new_dir_val), 0);
}

static int	execute_cd(char ***envp, int *envp_size, char **args, int i)
{
	char	*cwd;
	char	*previous_pwd;
	int		result;

	cwd = getcwd(NULL, 0);
	if (!cwd && i == 2 && access(args[1], F_OK))
		return (display_err_with_2_prefixes(WRONG_CWD_1, WRONG_CWD_2, \
			" No such file or directory\n"), perror(args[0]), 1);
	if (get_value_from_envp(&previous_pwd, "PWD", *envp_size, *envp) == 3)
		return (free(cwd), -3);
	if (i == 1)
		result = change_directory("HOME", *envp, *envp_size);
	else if (i == 2 && args[1][0] == '-')
		result = change_directory("OLDPWD", *envp, *envp_size);
	if (i == 1 || (i == 2 && args[1][0] == '-'))
	{
		if (result == -3)
			return (free(previous_pwd), free(cwd), -3);
		else if (result < 0)
			return (free(previous_pwd), free(cwd), 1);
	}
	else if (i == 2 && args[1][0] != '-' && chdir(args[1]) == -1)
		return (perror(args[1]), free(previous_pwd), free(cwd), 1);
	return (free(cwd), update_oldpwd_pwd_vars_in_envp(envp, envp_size, \
		&previous_pwd));
}

int	built_cd(char ***envp, int *envp_size, char **args, int fd)
{
	int		i;
	int		result;
	char	*new_dir_val;

	if (!(envp && *envp))
		return (-1);
	i = 0;
	while (args[i])
		i++;
	if (i > 2)
		return (display_err_with_prefix("cd", " too many arguments\n"), 1);
	else if (i == 2 && ft_strlen(args[1]) > 1 && args[1][0] == '-')
		return (display_err_with_2_prefixes(args[0], args[1], \
						" invalid option\n"), 2);
	result = execute_cd(envp, envp_size, args, i);
	new_dir_val = getcwd(NULL, 0);
	if (result == 0 && i == 2 && args[1][0] == '-' && new_dir_val)
	{
		write(fd, new_dir_val, ft_strlen(new_dir_val));
		write(fd, "\n", 1);
	}
	if (result == 0 && i == 2 && args[1][0] == '-' && !new_dir_val)
		return (display_err_with_prefix(args[0], \
				" error retrieving current directory\n"), 1);
	return (free(new_dir_val), result);
}
