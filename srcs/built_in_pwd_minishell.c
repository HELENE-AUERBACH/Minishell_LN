/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_pwd_minishell.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmorice <rmorice@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 15:33:42 by hauerbac          #+#    #+#             */
/*   Updated: 2024/09/23 12:00:00 by rmorice          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* ************************************************************************** */
/*                           copy_pwd_var_into_env                            */
/* -------------------------------------------------------------------------- */
/* This function extracts the Current Working Directory and copies it in      */
/* first place of envp array                                                  */
/* Inputs :                                                                   */
/*  - char **envp : the system environment datas                              */
/*  - int envp_size : the size of the envp array                              */
/* Return :                                                                   */
/*  - None                                                                    */
/* ************************************************************************** */
void	copy_pwd_var_into_env(char **envp, int envp_size)
{
	char	*pathname;
	char	*pwd_var;
	size_t	pwd_var_len;

	pathname = getcwd(NULL, 0);
	if (pathname && envp && envp_size > 0)
	{
		pwd_var = ft_strjoin("PWD=", pathname);
		if (pwd_var)
		{
			pwd_var_len = ft_strlen(pwd_var);
			envp[0] = (char *) malloc((pwd_var_len + 1) \
							* sizeof(char));
			if (envp[0])
				ft_strlcpy(envp[0], pwd_var, pwd_var_len + 1);
			free(pwd_var);
		}
		free(pathname);
	}
}

/* ************************************************************************** */
/*                                 built_pwd                                  */
/* -------------------------------------------------------------------------- */
/* This function displays the Current Working Directory into fd output        */
/* Inputs :                                                                   */
/*  - char **args : array that contained the arguments associated to builtin  */
/*  - int fd : the output file descriptor                                     */
/* Return :                                                                   */
/*  - 0 : if everything goes well                                             */
/*  - int : the error code of the problem encounter                           */
/* ************************************************************************** */
int	built_pwd(char **args, int fd)
{
	char	*pathname;

	if (args[1] && ft_strlen(args[1]) > 1 && args[1][0] == '-')
		return (display_err_with_2_prefixes(args[0], args[1], \
						" invalid option\n"), 2);
	pathname = getcwd(NULL, 0);
	if (!pathname)
	{
		if (errno == ENOMEM || errno == ERANGE)
			return (3);
		else
			return (display_err_with_2_prefixes(args[0], \
				" error retrieving current directory", \
				" getcwd: cannot access parent directories\n"),
				1);
	}
	write(fd, pathname, ft_strlen(pathname));
	write(fd, "\n", 1);
	free(pathname);
	return (0);
}
