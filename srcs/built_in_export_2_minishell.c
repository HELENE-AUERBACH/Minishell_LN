/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_export_2_minishell.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:39:38 by hauerbac          #+#    #+#             */
/*   Updated: 2024/07/01 15:16:31 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	var_name_found_in_env(char *envp_at_idx_i, char *var_name,
		size_t var_name_len)
{
	size_t	len;

	len = ft_strlen(envp_at_idx_i);
	if ((len >= var_name_len && var_name[var_name_len - 1] != '=' \
		&& ft_strncmp(var_name, envp_at_idx_i, var_name_len) == 0 \
			&& (envp_at_idx_i[var_name_len] == '\0' \
				|| envp_at_idx_i[var_name_len] == '=')) \
		|| (len >= var_name_len - 1 \
			&& var_name[var_name_len - 1] == '=' \
		&& ft_strncmp(var_name, envp_at_idx_i, var_name_len - 1) == 0 \
			&& (envp_at_idx_i[var_name_len - 1] == '\0' \
				|| envp_at_idx_i[var_name_len - 1] == '=')))
		return (1);
	else
		return (0);
}

static int	copy_envp(char ***envp, int *envp_size, int i)
{
	char	**tab;
	size_t	len;
	int		j;

	tab = (char **) malloc((i + 1 + 1) * sizeof(char *));
	if (!tab)
		return (-3);
	j = 0;
	while (j < i && j < *envp_size && (*envp)[j])
	{
		len = ft_strlen((*envp)[j]);
		tab[j] = (char *) malloc((len + 1) * sizeof(char));
		if (!tab[j])
			return (-3);
		ft_strlcpy(tab[j], (*envp)[j], len + 1);
		j++;
	}
	tab[i + 1] = NULL;
	free_tab(envp);
	*envp = tab;
	*envp_size = i + 1;
	return (0);
}

int	update_envp_with_var(char ***envp, int *envp_size, char *var, int i)
{
	int		return_code;
	size_t	var_len;

	return_code = 0;
	var_len = ft_strlen(var);
	if (i < *envp_size)
		free((*envp)[i]);
	else if (i != *envp_size)
		return_code = 1;
	else
	{
		return_code = copy_envp(envp, envp_size, i);
		if (return_code == -3)
			return (-3);
	}
	if (return_code == 0)
	{
		(*envp)[i] = (char *) malloc((var_len + 1) * sizeof(char));
		if (!(*envp)[i])
			return (-3);
		ft_strlcpy((*envp)[i], var, var_len + 1);
	}
	return (return_code);
}
