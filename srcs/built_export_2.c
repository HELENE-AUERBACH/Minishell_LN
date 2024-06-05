/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_export_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbocktor <jbocktor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 12:54:16 by jbocktor          #+#    #+#             */
/*   Updated: 2024/06/05 15:07:52 by jbocktor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	create_list_by_export(char *tab, t_list **to_list)
{
	t_list	*new;
	int		size;
	int		i;
	char	*content;

	i = 0;
	size = 0;
	content = ft_strdup(tab);
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
	return (size);
}

static int	is_allowed(char *s)
{
	int	i;

	i = 1;
	if (!ft_isalpha(s[0]) && s[0] != '_')
		return (1);
	while (s[i] != '\0' && s[i] != '=' && s[i] != '_')
	{
		if (!ft_isalnum(s[i]))
			return (1);
		i++;
	}
	return (0);
}

int	modify_environement(char **export, t_list **to_list)
{
	int		i;
	int		delta;
	t_list	*read;

	i = 0;
	delta = 0;
	while (export[i])
	{
		if (is_allowed(export[i]) == 1)
		{
			display_err_with_2_prefixes("export", export[i],
				" not a valid identifier\n");
			i++;
			continue ;
		}
		read = (*to_list);
		if (if_have_to_change(read, to_list, export[i]) == 0)
			delta += create_list_by_export(export[i], to_list);
		i++;
	}
	return (delta);
}
