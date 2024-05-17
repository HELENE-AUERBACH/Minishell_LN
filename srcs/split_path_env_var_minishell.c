/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_path_env_var_minishell.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 14:46:04 by hauerbac          #+#    #+#             */
/*   Updated: 2024/05/14 15:50:19 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
