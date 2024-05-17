/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_export.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbocktor <jbocktor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 12:54:16 by jbocktor          #+#    #+#             */
/*   Updated: 2024/05/17 14:49:05 by jbocktor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	there_is_a_dollar(char *env_arg)
{
	int	i;

	i = 0;
	while (env_arg[i])
	{
		if (env_arg[i] == '?')
			return (i);
		i++;
	}
	return (0);
}

int	there_is_a_number(char *env_arg)
{
	int	i;

	i = 0;
	while (env_arg[i])
	{
		if (env_arg[i] >= '0' && env_arg[i] <= '9')
			return (i);
		i++;
	}
	return (0);
}

static int	create_list_by_tab(char **tab, t_list **to_list)
{
	t_list	*new;
	int		size;
	int		i;

	i = 0;
	size = 0;
	while (tab[i++])
	{
		new = ft_lstnew(tab[i]);
		if (!new)
			return (-3);
		if (!(*to_list))
			(*to_list) = new;
		else
			ft_lstadd_back(to_list, new);
		size++;
	}
	return (size);
}

static int	new_environement(char ***envp, int *envp_size, char **export)
{
	int		delta;
	char	**realoc;
	t_list	*to_list;

	delta = 0;
	to_list = NULL;
	if (create_list_by_tab(*envp, &to_list) == -3)
		return (-3);
	delta += create_list_by_tab(&export[1], &to_list);
	if (delta == -3)
		return (-3);
	else
		(*envp_size) += delta;
	realoc = (char **)malloc(sizeof(char *) * (*envp_size + 1));
	if (!realoc)
		return (-3);
	copy_list_into_tab_and_free_list(realoc, &to_list, *envp_size);
	realoc[(*envp_size)] = NULL;
	free(envp);
	envp = &realoc;
	return (0);
}

int	built_export(char ***envp, int *envp_size, char **export)
{
	int	i;
	int	fd;

	fd = 1;
	i = 0;
	while (export[i])
		i++;
	if (i == 1)
	{
		while (*envp[i++])
			write(fd, *envp[i], ft_strlen(*envp[i]));
		return (0);
	}
	if (new_environement(envp, envp_size, export) == -3)
		return (3);
	return (0);
}
