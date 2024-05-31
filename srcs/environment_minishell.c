/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_minishell.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 17:03:43 by hauerbac          #+#    #+#             */
/*   Updated: 2024/05/31 12:20:55 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_val_from_envp(char **value, char *name, int envp_size,
			char **envp)
{
	int		i;
	size_t	name_len;
	size_t	val_len;

	*value = NULL;
	i = -1;
	name_len = 0;
	if (name)
		name_len = ft_strlen(name);
	if (!name || name_len == 0)
		return (0);
	while (++i < envp_size && envp[i] && envp[i][0] != '\0')
	{
		if (ft_strncmp(name, envp[i], name_len) == 0
			&& envp[i][name_len] == '=')
		{
			val_len = ft_strlen(envp[i]) - name_len - 1;
			*value = (char *) malloc((val_len + 1) * sizeof(char));
			if (!*value)
				return (3);
			ft_strlcpy(*value, envp[i] + name_len + 1, val_len + 1);
			break ;
		}
	}
	return (0);
}

int	get_value_from_envp(char **value, char *name, int envp_size, char **envp)
{
	size_t	name_len;

	*value = NULL;
	name_len = 0;
	if (name)
		name_len = ft_strlen(name);
	if (!name || name_len == 0)
		return (0);
	if (name_len == 1 && name[0] == '?')
	{
		*value = ft_itoa(g_exit_status);
		if (!*value)
			return (3);
		return (0);
	}
	else
		return (get_val_from_envp(value, name, envp_size, envp));
}
