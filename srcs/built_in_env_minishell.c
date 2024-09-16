/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_env_minishell.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmorice <rmorice@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 14:20:12 by hauerbac          #+#    #+#             */
/*   Updated: 2024/09/06 14:22:35 by rmorice          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* ************************************************************************** */
/*                        print_env_vars_with_a_value                         */
/* -------------------------------------------------------------------------- */
/* This function writes into fd every variables contained into envp that have */
/* an associated value (contained "=") follow by '\n'                         */
/* rq : if envp[i] begin with "_=" but isn't "_=env" then we replace envp[i]  */
/* by it                                                                      */
/* Inputs :                                                                   */
/*  - char **envp : the system environment datas                              */
/*  - int fd : the output file descriptor                                     */
/* Return :                                                                   */
/*  - None                                                                    */
/* ************************************************************************** */
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

/* ************************************************************************** */
/*                        is_there_a_special_parameter                        */
/* -------------------------------------------------------------------------- */
/* This function checks if envp contained a "special parameter" (one that     */
/* begin with "_=")                                                           */
/* Inputs :                                                                   */
/*  - char **envp : the system environment datas                              */
/* Return :                                                                   */
/*  - 1 : if envp contained a "special parameter"                             */
/*  - 0 : otherwise                                                           */
/* ************************************************************************** */
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

/* ************************************************************************** */
/*                                 built_env                                  */
/* -------------------------------------------------------------------------- */
/* This function writes into fd every variables contained into envp that have */
/* an associated value (contained "=") follow by '\n'                         */
/* rq : if no special parameter exist in envp then we write (into fd ouput)   */
/* "_=env\n" after the last variable                                          */
/* Inputs :                                                                   */
/*  - char **envp : the system environment datas                              */
/*  - char **args         */
/*  - int fd : the output file descriptor                                     */
/* Return :                                                                   */
/*  - 0 : if everything goes well                                             */
/*  - int : the error code of the problem encounter                           */
/* ************************************************************************** */
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
