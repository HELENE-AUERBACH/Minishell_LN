/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_cd_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbocktor <jbocktor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 13:08:44 by jbocktor          #+#    #+#             */
/*   Updated: 2024/05/29 13:24:01 by jbocktor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	transform_to_list(t_list **to_list, char ***envp, char *equal_exp,
		char *value)
{
	char	*content;
	t_list	*new;
	int		i;

	i = 0;
	while ((*envp)[i])
	{
		content = ft_strdup((*envp)[i]);
		if (!content)
			return (-3);
		new = ft_lstnew(content);
		if (!(*to_list))
			(*to_list) = new;
		else
			ft_lstadd_back(to_list, new);
		i++;
	}
	content = ft_strjoin(equal_exp, value);
	if (!content)
		return (-3);
	new = ft_lstnew(content);
	if (!(*to_list))
		(*to_list) = new;
	ft_lstadd_back(to_list, new);
	return (0);
}

int	add_value_for_pwd(char ***envp, char *equal_exp, int *envp_size,
		char *value)
{
	t_list	*to_list;
	char	**realoc;

	to_list = NULL;
	if (transform_to_list(&to_list, envp, equal_exp, value) == -3)
		return (-3);
	(*envp_size) += 1;
	realoc = (char **)malloc(sizeof(char *) * (*envp_size + 1));
	if (!realoc)
		return (-3);
	free_tab(envp);
	copy_list_into_tab_and_free_list(realoc, &to_list, *envp_size);
	realoc[(*envp_size)] = NULL;
	*envp = realoc;
	return (0);
}

int	change_value_for_pwd(char ***envp, char *exp, char *equal_exp, char *value)
{
	int	i;

	i = 0;
	if (!value)
		return (0);
	while ((*envp)[i])
	{
		if (ft_strncmp((*envp)[i], exp, ft_strlen(exp)) == 0)
		{
			free((*envp)[i]);
			(*envp)[i] = ft_strjoin(equal_exp, value);
			if ((*envp)[i] == NULL)
				return (-3);
			break ;
		}
		i++;
	}
	return (0);
}

int	modify_pwd_value(int *envp_size, char ***envp, char *new_value, char *exp)
{
	char	*value;
	char	*with_equal;

	with_equal = ft_strjoin(exp, "=");
	get_value_from_envp(&value, exp, *envp_size, *envp);
	if (value == NULL)
	{
		if (add_value_for_pwd(envp, with_equal, envp_size, new_value) == -3)
			return (-3);
	}
	else
	{
		if (change_value_for_pwd(envp, exp, with_equal, new_value) == -3)
			return (-3);
	}
	free(value);
	free(with_equal);
	return (0);
}
