/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_export_3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbocktor <jbocktor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 12:54:16 by jbocktor          #+#    #+#             */
/*   Updated: 2024/06/04 13:16:23 by jbocktor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strn_equal_cmp(const char *s1, const char *s2, size_t n)
{
	size_t	result;
	size_t	i;
	size_t	j;

	result = 0;
	i = 0;
	j = 0;
	while ((s1[i] != '\0' || s2[j] != '\0') && (s1[i] != '=' || s2[j] != '=')
		&& (i < n || j < n))
	{
		result = (unsigned char)s1[i] - (unsigned char)s2[j];
		if (result != 0)
			return (result);
		if (s1[i] != '\0' || s1[i] != '=')
			i++;
		if (s2[j] != '\0' || s2[i] != '=')
			j++;
	}
	return (result);
}

size_t	ft_str_equal_len(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0' && s[i] != '=')
	{
		i++;
	}
	return (i);
}

static int	there_is_an_equal(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '=')
			return (i);
		i++;
	}
	return (0);
}

static int	change_value(char *content, t_list **to_list)
{
	int		equal;
	char	*content_envp;
	t_list	*read;

	read = *(to_list);
	while (read)
	{
		content_envp = (char *)(read->content);
		if (ft_strn_equal_cmp(content_envp, content,
				ft_str_equal_len(content)) == 0)
		{
			equal = there_is_an_equal(content);
			if (equal)
			{
				free(read->content);
				read->content = ft_strdup(content);
				if (!read->content)
					return (-3);
			}
			break ;
		}
		read = read->next;
	}
	return (0);
}

int	if_have_to_change(t_list *read, t_list **to_list, char *export)
{
	char	*content_envp;

	while (read)
	{
		content_envp = (char *)(read->content);
		if (ft_strn_equal_cmp(content_envp, export,
				ft_str_equal_len(export)) == 0)
		{
			change_value(export, to_list);
			return (1);
		}
		read = read->next;
	}
	return (0);
}
