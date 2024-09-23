/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_minishell.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmorice <rmorice@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 17:03:43 by hauerbac          #+#    #+#             */
/*   Updated: 2024/09/23 13:01:06 by rmorice          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* ************************************************************************** */
/*                             get_val_from_envp                              */
/* -------------------------------------------------------------------------- */
/* This function extracts the value associated at <name> variable in envp     */
/* The variable "value" is update to NULL if name doesn't exist in envp or    */
/* envp is NULL                                                               */
/* Inputs :                                                                   */
/*  - char **value : pointer to a string that will contained the value found  */
/*  - char *name : the name of the var for which we want the associated value */
/*  - int envp_size : the size of the char **envp variable                    */
/*  - char **envp : the system environment (of our current shell)             */
/* Return :                                                                   */
/*  - 0 : if everything goes well                                             */
/*  - 3 : otherwise (malloc error)                                            */
/* ************************************************************************** */
// 0 if everything goes well I suppose...
// but why do we return 0 if !name of name == '\0' ???
// do we suppose that <name> exists in the environment (as we don't check if it
// was found in the environment ???) or we don't care as if the name doesn't
// exist then value == NULL so it will be taken into account later on ???
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

/* ************************************************************************** */
/*                            get_value_from_envp                             */
/* -------------------------------------------------------------------------- */
/* This function looks for the value associated to the <name> variable in the */
/* environment datas. The variable "value" is update to NULL if name doesn't  */
/* exist in envp or envp is NULL                                              */
/* if name is NULL or void then 0 is return                                   */
/* if name is "?\0" then -1 is return                                         */
/* otherwise, we extract the value associated with <name> in envp.            */
/* Inputs :                                                                   */
/*  - char **value : pointer to a string that will contained the value found  */
/*  - char *name : the name of the var for which we want the associated value */
/*  - int envp_size : the size of the char **envp variable                    */
/*  - char **envp : the system environment (of our current shell)             */
/* Return :                                                                   */
/*  - 0 : if everything goes well                                             */
/*  - 3 : otherwise (malloc error)                                            */
/* ************************************************************************** */
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
		return (-1);
	else
		return (get_val_from_envp(value, name, envp_size, envp));
}

/* ************************************************************************** */
/*                       get_value_from_minishell_envp                        */
/* -------------------------------------------------------------------------- */
/* This function looks for the value associated to the <name> variable in the */
/* current environment.                                                       */
/* if name == ? then we are looking for the exit code of the last command     */
/* Inputs :                                                                   */
/*  - char **value : pointer to a string that will contained the value found  */
/*  - char *name : the name of the var for which we want the associated value */
/*  - t_data *d : a structure that contained datas about the shell & cmd line */
/* Return :                                                                   */
/*  - 0 : if everything goes well                                             */
/*  - 3 : otherwise (malloc error)                                            */
/* ************************************************************************** */
int	get_value_from_minishell_envp(char **value, char *name, t_data *d)
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
		*value = ft_itoa(d->return_code);
		if (!*value)
			return (3);
		return (0);
	}
	else
		return (get_val_from_envp(value, name, d->envp_size, d->envp));
}
