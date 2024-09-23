/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_unset_minishell.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmorice <rmorice@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 16:45:56 by hauerbac          #+#    #+#             */
/*   Updated: 2024/09/23 12:00:16 by rmorice          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* ************************************************************************** */
/*                       var_name_found_in_env_at_idx_i                       */
/* -------------------------------------------------------------------------- */
/* This function checks if var is found at the beginning of envp_at_idx_i and */
/* if the first character after the comparison is either "=" or "\0"          */
/* rq : envp_at_idx_i is the input that correpond to envp at the index i      */
/* Inputs :                                                                   */
/*  - char *envp_at_idx_i : the string contained at index i of envp           */
/*  - char *var : the string that we are looking for in envp_at_idx_i         */
/*  - size_t var_name_len : the length of the string var                      */
/* Return :                                                                   */
/*  - 0 : if everything goes well                                             */
/*  - int : the error code of the problem encounter                           */
/* ************************************************************************** */
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

/* ************************************************************************** */
/*                          free_from_envp_at_idx_i                           */
/* -------------------------------------------------------------------------- */
/* This function frees envp[i] and set it to NULL                             */
/* Inputs :                                                                   */
/*  - char ***envp : a pointer to an array of string about the environment    */
/*  - int i : the index of the element of envp to free and set to NULL        */
/* Return :                                                                   */
/*  - None                                                                    */
/* ************************************************************************** */
static void	free_from_envp_at_idx_i(char ***envp, int i)
{
	free((*envp)[i]);
	(*envp)[i] = NULL;
}

/* ************************************************************************** */
/*                            delete_var_from_envp                            */
/* -------------------------------------------------------------------------- */
/* This function searchs for the index corresponding to var in envp then      */
/* envp[i] is freed and point to NULL and all the datas of envp that have a   */
/* greater index are copy one index prior (remove the void string created)    */
/* The environment size is update (decreased by one)                          */
/* Inputs :                                                                   */
/*  - char ***envp : a pointer to an array of string about the environment    */
/*  - int *envp_size : a pointer to the size of the array of strings envp     */
/*  - char *var : a string that contained a var name that might exist in envp */
/* Return :                                                                   */
/*  - 0 : if everything goes well                                             */
/*  - int : the error code of the problem encounter (-3 malloc error)         */
/* ************************************************************************** */
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

/* ************************************************************************** */
/*                                built_unset                                 */
/* -------------------------------------------------------------------------- */
/* This function deletes every variables and associated datas contained in    */
/* args (but the first) from the environment. That means that we remove every */
/* element associated to the arguments of unset in the environment            */
/* Inputs :                                                                   */
/*  - char ***envp : a pointer to an array of string about the environment    */
/*  - int *envp_size : a pointer to the size of the array of strings envp     */
/*  - char **args : array that contained the arguments associated to builtin  */
/* Return :                                                                   */
/*  - 0 : if everything goes well                                             */
/*  - int : the error code of the problem encounter                           */
/* ************************************************************************** */
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
