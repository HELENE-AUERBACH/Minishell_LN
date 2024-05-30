/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_export.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbocktor <jbocktor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 12:54:16 by jbocktor          #+#    #+#             */
/*   Updated: 2024/05/30 14:33:32 by jbocktor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	create_list_by_environement(char **tab, t_list **to_list)
{
	t_list	*new;
	int		size;
	int		i;
	char	*content;

	i = 0;
	size = 0;
	while (tab[i])
	{
		content = ft_strdup(tab[i]);
		if (!content)
			return (-3);
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
	if (create_list_by_environement(*envp, &to_list) == -3)
		return (-3);
	free_tab(envp);
	delta += create_list_by_environement(&export[1], &to_list);
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

void	print_export(char ***envp, int i, int fd)
{
	int		y;
	char	*string;

	string = (*envp)[i];
	y = 0;
	string = (*envp)[i];
	while (string[y] && string[y] != '=')
		write(fd, &string[y++], 1);
	if (string[y] == '=' && !string[y + 1])
	{
		write(fd, &string[y++], 1);
		write(fd, "\"\"", 2);
	}
	else if (string[y] == '=')
	{
		write(fd, &string[y++], 1);
		write(fd, "\"", 1);
		while (string[y])
			write(fd, &string[y++], 1);
		write(fd, "\"", 1);
	}
}

int	built_export(char ***envp, int *envp_size, char **export, int fd)
{
	int	i;

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
			print_export(envp, i, fd);
			write(fd, "\n", 1);
			i++;
		}
		return (0);
	}
	if (new_environement(envp, envp_size, export) == -3)
		return (3);
	return (0);
}
