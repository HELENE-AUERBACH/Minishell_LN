/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_export_minishell.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 15:08:25 by hauerbac          #+#    #+#             */
/*   Updated: 2024/07/03 14:06:47 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_env_var(char *env_var, size_t env_var_len, int fd)
{
	size_t	j;
	int		sign_found;

	if (env_var_len > 0 \
		&& !(env_var_len == 5 \
			&& ft_strncmp("_=env", env_var, 5) == 0))
	{
		write(fd, "export ", 7);
		sign_found = 0;
		j = 0;
		while (j < env_var_len)
		{
			write(fd, &env_var[j], 1);
			if (sign_found == 0 && env_var[j] == '=')
			{
				sign_found = 1;
				write(fd, "\"", 1);
			}
			j++;
		}
		if (sign_found)
			write(fd, "\"", 1);
		write(fd, "\n", 1);
	}
}

static int	split_var_name_and_value(int i, char **args, char **vns,
				char **vvals)
{
	int	return_code;
	int	j;

	return_code = 0;
	j = 0;
	while (args[i][j] && (j == 0 || (j > 0 && args[i][j] != '=')))
	{
		if (((j == 0 && !ft_isalpha(args[i][j])) || (j > 0 \
			&& !ft_isalnum(args[i][j]))) && args[i][j] != '_')
		{
			display_err_with_2_prefixes("export", args[i], \
						" not a valid identifier\n");
			return_code = 1;
		}
		j++;
	}
	if (args[i][j] && args[i][j] == '=')
		j++;
	vns[i] = (char *) malloc((j + 1) * sizeof(char));
	vvals[i] = (char *) malloc((ft_strlen(args[i]) - j + 1) * sizeof(char));
	if (!vns[i] || !vvals[i])
		return (-3);
	ft_strlcpy(vns[i], args[i], j + 1);
	ft_strlcpy(vvals[i], args[i] + j, ft_strlen(args[i]) - j + 1);
	return (return_code);
}

int	updenv(char ***envp, int *envp_size, char *var_name, char *var_value)
{
	int		i;
	int		return_code;
	size_t	var_name_len;
	char	*var;

	return_code = 0;
	i = 0;
	var_name_len = ft_strlen(var_name);
	var = (char *) malloc((var_name_len + 1) * sizeof(char));
	if (!var)
		return (-3);
	ft_strlcpy(var, var_name, var_name_len + 1);
	if (var && var_value && ft_strlen(var_value) > 0)
		var = ft_strjoin_with_free_s1(var, var_value);
	if (!var)
		return (-3);
	while (i < *envp_size && (*envp)[i] \
		&& !var_name_found_in_env((*envp)[i], var_name, var_name_len))
			i++;
	return_code = update_envp_with_var(envp, envp_size, var, i);
	free(var);
	return (return_code);
}

static int	export_vars(char ***envp, int *envp_size, char **args, int nv)
{
	int		i;
	int		return_code;
	int		final_return_code;
	char	**vns;
	char	**vvals;

	final_return_code = 0;
	vns = (char **) malloc(nv * sizeof(char *));
	vvals = (char **) malloc(nv * sizeof(char *));
	if (!vns || !vvals)
		return (-3);
	i = 0;
	while (i < nv && args[i] && final_return_code != -3)
	{
		vns[i] = NULL;
		vvals[i] = NULL;
		return_code = split_var_name_and_value(i, args, vns, vvals);
		if (return_code == 0)
			return_code = updenv(envp, envp_size, vns[i], vvals[i]);
		if (return_code != 0 && final_return_code == 0)
			final_return_code = return_code;
		i++;
	}
	return (free_tab_with_size(&vns, nv), free_tab_with_size(&vvals, nv), \
		final_return_code);
}

int	built_export(char ***envp, int *envp_size, char **args, int fd)
{
	int	i;
	int	found;

	if (!(envp && *envp))
		return (-1);
	i = 0;
	while (args[i])
		i++;
	if (i > 1 && args[1][0] == '-')
		return (display_err_with_2_prefixes("export", args[1], \
			" invalid option\n"), 2);
	if (i > 1)
		return (export_vars(envp, envp_size, &(args[1]), i - 1));
	i = 0;
	found = 0;
	while (i < *envp_size && (*envp)[i] != NULL)
	{
		print_env_var((*envp)[i], ft_strlen((*envp)[i]), fd);
		if (ft_strncmp("OLDPWD=", (*envp)[i], 7) == 0)
			found = 1;
		i++;
	}
	if (!found)
		write(fd, "export OLDPWD\n", 14);
	return (0);
}
