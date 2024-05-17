/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_unset.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbocktor <jbocktor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 13:09:13 by jbocktor          #+#    #+#             */
/*   Updated: 2024/05/17 14:50:33 by jbocktor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_set(char **unset, char *envp)
{
	int	i;

	i = 1;
	while (unset[i])
		if (strncmp(unset[i], envp, ft_strlen(unset[i])))
			return (0);
	return (1);
}

static int	create_list_by_tab(char ***envp, char **unset, t_list **to_list)
{
	t_list	*new;
	int		size;
	int		i;

	size = 0;
	i = 0;
	while (*envp[i++])
	{
		if (is_set(unset, *envp[i]))
		{
			new = ft_lstnew(*envp[i]);
			if (!new)
				return (-3);
			if (!to_list)
				(*to_list) = new;
			else
				ft_lstadd_back(to_list, new);
		}
		else
			size++;
	}
	return (size);
}

int	built_unset(char ***envp, int *envp_size, char **unset)
{
	int		i;
	int		delta;
	char	**realoc;
	t_list	*to_list;

	i = 0;
	to_list = NULL;
	delta = create_list_by_tab(envp, unset, &to_list);
	if (delta == -3)
		return (3);
	else
		(*envp_size) -= delta;
	realoc = (char **)malloc(sizeof(char *) * (*envp_size + 1));
	if (!realoc)
		return (3);
	copy_list_into_tab_and_free_list(realoc, &to_list, *envp_size);
	realoc[(*envp_size)] = NULL;
	free(envp);
	envp = &realoc;
	return (0);
}
