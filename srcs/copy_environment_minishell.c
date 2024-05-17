/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_environment_minishell.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 18:51:27 by hauerbac          #+#    #+#             */
/*   Updated: 2024/05/17 14:37:47 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_environment_size(char **envp)
{
	int	size;
	int	i;

	size = 0;
	if (envp)
	{
		i = 0;
		while (envp[i])
		{
			if (envp[i][0] != '\0')
				size++;
			i++;
		}
	}
	return (size);
}

static char	*get_new_shell_level(void)
{
	char	*shlvl_new;
	char	*shlvl;
	char	*shlvl_new_val;

	shlvl_new = NULL;
	shlvl = getenv("SHLVL");
	if (!shlvl)
		shlvl_new = ft_strjoin("SHLVL=", "1");
	else
	{
		shlvl_new_val = ft_itoa(ft_atoi(shlvl) + 1);
		if (shlvl_new_val)
		{
			shlvl_new = ft_strjoin("SHLVL=", shlvl_new_val);
			free(shlvl_new_val);
		}
	}
	return (shlvl_new);
}

static void	exit_when_error_on_envp_copy(t_data *d, int j)
{
	while (j > 0)
		free(d->envp[--j]);
	free(d->envp);
	d->envp = NULL;
	display_error("Environment copy error\n");
	exit (1);
}

static void	copy_environment_strings(t_data *d, char **envp, char *shlvl)
{
	int		i;
	int		j;
	size_t	len;
	char	*src;

	i = 0;
	j = 0;
	while (envp[i] && j < d->envp_size)
	{
		if (envp[i][0] != '\0')
		{
			src = shlvl;
			if (ft_strncmp("SHLVL=", envp[i], 6) != 0)
				src = envp[i];
			len = ft_strlen(src);
			d->envp[j] = (char *) malloc((len + 1) * sizeof(char));
			if (!d->envp[j])
				exit_when_error_on_envp_copy(d, j);
			ft_strlcpy(d->envp[j++], src, len + 1);
		}
		i++;
	}
	while (j < d->envp_size + 1)
		d->envp[j++] = NULL;
	return ;
}

void	copy_environment(t_data *d, char **envp)
{
	int		to_be_copied;
	char	*shlvl_new;

	d->envp_size = get_environment_size(envp);
	d->envp = NULL;
	to_be_copied = 0;
	shlvl_new = get_new_shell_level();
	if (d->envp_size == 0)
		d->envp_size = 1;
	else
		to_be_copied = 1;
	d->envp = (char **) malloc((d->envp_size + 1) * sizeof(char *));
	if (d->envp && to_be_copied)
		copy_environment_strings(d, envp, shlvl_new);
	else if (d->envp && !to_be_copied)
	{
		d->envp[0] = (char *) malloc((7 + 1) * sizeof(char));
		if (!d->envp[0])
			exit_when_error_on_envp_copy(d, 0);
		ft_strlcpy(d->envp[0], shlvl_new, 7 + 1);
	}		
	if (shlvl_new)
		free(shlvl_new);
	return ;
}
