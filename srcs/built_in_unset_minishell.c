/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_unset_minishell.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 16:45:56 by hauerbac          #+#    #+#             */
/*   Updated: 2024/07/01 18:30:23 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	var_name_found_in_env_at_idx_i(char *envp_at_idx_i, char *var,
			size_t var_name_len)
{
	size_t	len;

	len = ft_strlen(envp_at_idx_i);
	if (len >= var_name_len && var[var_name_len] == '\0'\
		&& ft_strncmp(var, envp_at_idx_i, var_name_len) == 0 \
		&& (envp_at_idx_i[var_name_len] == '\0' \
			|| envp_at_idx_i[var_name_len] == '='))
		return (1);
	else
		return (0);
}

static void	free_from_envp_at_idx_i(char ***envp, int i)
{
	free((*envp)[i]);
	(*envp)[i] = NULL;
}

static int	delete_var_from_envp(char ***envp, int *envp_size, char *var)
{
	int		i;
	size_t	len;

	len = 0;
	while (var[len] && var[len] != '=')
		len++;
	i = 0;
	while (i < *envp_size && (*envp)[i] \
		&& !var_name_found_in_env_at_idx_i((*envp)[i], var, len))
		i++;
	if (!(i < *envp_size && (*envp)[i]))
		return (0);
	free_from_envp_at_idx_i(envp, i);
	while (i + 1 < *envp_size && (*envp)[i + 1])
	{
		len = ft_strlen((*envp)[i + 1]);
		(*envp)[i] = (char *) malloc((len + 1) * sizeof(char));
		if (!(*envp)[i])
			return (-3);
		ft_strlcpy((*envp)[i], (*envp)[i + 1], len + 1);
		free_from_envp_at_idx_i(envp, i + 1);
		i++;
	}
	*envp_size = *envp_size - 1;
	return (0);
}

int	built_unset(char ***envp, int *envp_size, char **args)
{
	int	i;
	int	final_return_code;
	int	return_code;

	if (!(envp && *envp))
		return (-1);
	i = 0;
	final_return_code = 0;
	while (args[i])
	{
		i++;
		if (i > 1 && args[1][0] == '-')
			return (display_err_with_2_prefixes("unset", args[1], \
				" invalid option\n"), 2);
		if (i > 1)
		{
			return_code = delete_var_from_envp(envp, envp_size, \
								args[i - 1]);
			if (return_code != 0 && final_return_code == 0)
				final_return_code = return_code;
		}
	}
	return (final_return_code);
}
