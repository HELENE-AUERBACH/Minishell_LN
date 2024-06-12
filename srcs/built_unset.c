/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_unset.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbocktor <jbocktor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 13:09:13 by jbocktor          #+#    #+#             */
/*   Updated: 2024/06/12 12:32:13 by jbocktor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_set(char **unset, char *envp)
{
	int	i;
	int	len_envp;
	int	len_unset;

	len_envp = ft_strlen(envp);
	i = 1;
	while (unset[i])
	{
		len_unset = ft_strlen(unset[i]);
		if (ft_strncmp(unset[i], envp, len_unset) == 0)
		{
			if (len_unset == len_envp)
				return (0);
			else if (envp[len_unset] == '=')
				return (0);
		}
		i++;
	}
	return (1);
}

static int	create_new_list(t_list **new, char ***envp, int pos)
{
	char	*content;

	content = ft_strdup((*envp)[pos]);
	if (!content)
		return (-3);
	(*new) = ft_lstnew(content);
	if (!(*new))
		return (-3);
	return (0);
}

static int	create_list_by_tab(char ***envp, char **unset, t_list **to_list)
{
	t_list	*new;
	int		size;
	int		i;

	size = 0;
	i = 0;
	while ((*envp)[i])
	{
		if (is_set(unset, (*envp)[i]) != 0)
		{
			if (create_new_list(&new, envp, i) == -3)
				return (-3);
			if (!to_list)
				(*to_list) = new;
			else
				ft_lstadd_back(to_list, new);
		}
		else
			size++;
		i++;
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
	if (unset[1] && unset[1][0] == '-')
		return (display_err_with_2_prefixes(unset[0], unset[1],
				" invalid option\n"), 1);
	delta = create_list_by_tab(envp, unset, &to_list);
	if (delta == -3)
		return (3);
	else
		(*envp_size) -= delta;
	free_tab(envp);
	realoc = (char **)malloc(sizeof(char *) * (*envp_size + 1));
	if (!realoc)
		return (3);
	copy_list_into_tab_and_free_list(realoc, &to_list, *envp_size);
	realoc[(*envp_size)] = NULL;
	*envp = realoc;
	return (0);
}
