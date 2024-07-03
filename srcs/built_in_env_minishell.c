/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_env_minishell.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 14:20:12 by hauerbac          #+#    #+#             */
/*   Updated: 2024/06/28 12:31:38 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_env_vars_with_a_value(char **envp, int fd)
{
	int		i;
	size_t	j;
	size_t	env_var_len;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp("_=", envp[i], 2) == 0 \
			&& ft_strncmp("_=env", envp[i], 5) != 0)
			ft_strlcpy(envp[i], "_=env", 6);
		j = 0;
		env_var_len = ft_strlen(envp[i]);
		while (j < env_var_len)
		{
			if (envp[i][j] == '=')
			{
				write(fd, envp[i], env_var_len);
				write(fd, "\n", 1);
				break ;
			}
			j++;
		}
		i++;
	}
}

static int	is_there_a_special_parameter(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp("_=", envp[i], 2) == 0)
			return (1);
		i++;
	}
	return (0);
}

int	built_env(char **envp, char **args, int fd)
{
	int	i;

	i = 1;
	while (args[i])
		i++;
	if (i > 1 && ft_strlen(args[1]) > 1 && args[1][0] == '-')
		return (display_err_with_2_prefixes(args[0], args[1], \
						" invalid option\n"), 125);
	else if (i > 1 && ft_strlen(args[1]) == 1 && args[1][0] == '-')
		return (0);
	else if (i > 1)
		return (display_err_with_prefix(args[1], \
						" No such file or directory\n"),
			127);
	if (envp)
	{
		print_env_vars_with_a_value(envp, fd);
		if (!is_there_a_special_parameter(envp))
			write(fd, "_=env\n", 6);
	}
	return (0);
}
