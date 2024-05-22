/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_export.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbocktor <jbocktor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 12:54:16 by jbocktor          #+#    #+#             */
/*   Updated: 2024/05/22 20:19:41 by hauerbac         ###   ########.fr       */
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

/*
int already_exist(t_list *to_list, char *string)
{
	t_list *read;
	
	read = to_list;
	while(to_list)
	{
		if (ft_strncmp(string, (char *)(read->content), ft_strlen(string)));
			return()
		read = read->next;
	}
}
*/

static int	create_list_by_tab(char **tab, t_list **to_list)
{
	t_list	*new;
	int		size;
	int		i;
	char	*content;

	i = 0;
	size = 0;
	while (tab[i])
	{
		content = (char *) malloc(sizeof(char) * (ft_strlen(tab[i]) + 1));
		if (!content)
			return (-3);
		ft_strlcpy(content, tab[i], ft_strlen(tab[i]) + 1);
		new = ft_lstnew(content);
		if (!new)
			return (-3);
		if (!(*to_list))
			(*to_list) = new;
		else
			ft_lstadd_back(to_list, new);
		size++;
		i++;
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
	free_tab(envp);
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
	*envp = realoc;
	return (0);
}

int	built_export(char ***envp, int *envp_size, char **export, int fd)
{
	int		i;
	char	*string;

	i = 0;
	while (export[i])
		i++;
	if (i == 1)
	{
		i = 0;
		if (!(*envp))
			return (-1);
		while (i < *envp_size && (*envp)[i])
		{
			string = (*envp)[i];
			write(fd, string, ft_strlen(string));
			write(fd, "\n", 1);
			i++;
		}
		return (0);
	}
	if (new_environement(envp, envp_size, export) == -3)
		return (3);
	return (0);
}
