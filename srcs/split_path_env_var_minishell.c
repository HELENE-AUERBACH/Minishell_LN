/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_path_env_var_minishell.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmorice <rmorice@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 14:46:04 by hauerbac          #+#    #+#             */
/*   Updated: 2024/09/25 15:21:30 by rmorice          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* ************************************************************************** */
/*                               get_nb_colons                                */
/* -------------------------------------------------------------------------- */
/* This function calculates the number of paths contained in the PATH section */
/* of the environment datas                                                   */
/* To do so, we count the number of colons as each path is separated by a     */
/* colon                                                                      */
/* Inputs :                                                                   */
/*  - char **envp : the system environment datas                              */
/*  - char **path_env_var : a pointer to a str, a copy of PATH line from envp */
/* Return :                                                                   */
/*  - int : the number of colons (+1) => number of paths                      */
/* ************************************************************************** */
static int	get_nb_colons(char **envp, char **path_env_var)
{
	int	i;
	int	nb_colons;
	int	path_val_len;

	i = 0;
	while (envp[i] != NULL && ft_strncmp("PATH=", envp[i], 5) != 0)
		i++;
	if (!(envp[i] != NULL && ft_strncmp("PATH=", envp[i], 5) == 0))
		return (0);
	if (envp[i] != NULL && ft_strncmp("PATH=\0", envp[i], 6) == 0)
		return (0);
	path_val_len = ft_strlen(envp[i]) - 5;
	*path_env_var = (char *) malloc((path_val_len + 1) * sizeof(char));
	if (!*path_env_var)
		return (0);
	ft_strlcpy(*path_env_var, envp[i] + 5, path_val_len + 1);
	i = 0;
	nb_colons = 1;
	while (i < path_val_len && (*path_env_var)[i])
	{
		if ((*path_env_var)[i] == ':')
			nb_colons++;
		i++;
	}
	return (nb_colons);
}

/* ************************************************************************** */
/*                        split_path_env_var_into_tab                         */
/* -------------------------------------------------------------------------- */
/* This function splits the paths contained in path_env_var and copies each   */
/* path into tab, each path is separated by ":"                               */
/* rq : "." is added at the end of tab (a NULL terminated array)              */
/* Inputs :                                                                   */
/*  - char **tab : array of string that contained the cmd and its arg         */
/*  - char **path_env_var : a pointer to a str, a copy of PATH line from envp */
/* Return :                                                                   */
/*  - None                                                                    */
/* ************************************************************************** */
static void	split_path_env_var_into_tab(char **tab, char *path_env_var)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	j = 0;
	k = 0;
	while (path_env_var && i < (int) ft_strlen(path_env_var))
	{
		while (path_env_var[i] && path_env_var[i] != ':')
			i++;
		i++;
		tab[j] = (char *) malloc((i - k) * sizeof(char));
		if (!tab[j])
			break ;
		ft_strlcpy(tab[j], path_env_var + k, i - k);
		j++;
		k = i;
	}
	tab[j + 1] = NULL;
	tab[j] = (char *) malloc(2 * sizeof(char));
	if (!tab[j])
		free_tab(&tab);
	else
		ft_strlcpy(tab[j], ".", 2);
}

/* ************************************************************************** */
/*                                 get_paths                                  */
/* -------------------------------------------------------------------------- */
/* This function creates an array of strings that contained every possible    */
/* path from the environment                                                  */
/* Input :                                                                    */
/*  - char **envp : the system environment datas                              */
/* Return :                                                                   */
/*  - char **: the array created                                              */
/* ************************************************************************** */
char	**get_paths(char **envp)
{
	char	**tab;
	char	*path_env_var;
	int		nb_colons;

	path_env_var = NULL;
	if (!envp || envp[0] == NULL || envp[0][0] == '\0')
		nb_colons = 0;
	else
		nb_colons = get_nb_colons(envp, &path_env_var);
	tab = (char **) malloc(sizeof(char *) * (nb_colons + 1 + 1));
	if (!tab)
	{
		if (path_env_var)
			free(path_env_var);
		path_env_var = NULL;
		display_error("Malloc error for PATH environment variable\n");
		return (NULL);
	}
	split_path_env_var_into_tab(tab, path_env_var);
	if (path_env_var)
		free(path_env_var);
	path_env_var = NULL;
	if (!tab)
		display_error("Malloc error for PATH environment variable\n");
	return (tab);
}
