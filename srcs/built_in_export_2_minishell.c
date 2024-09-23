/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_export_2_minishell.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmorice <rmorice@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:39:38 by hauerbac          #+#    #+#             */
/*   Updated: 2024/09/23 11:42:27 by rmorice          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* ************************************************************************** */
/*                           var_name_found_in_env                            */
/* -------------------------------------------------------------------------- */
/* This function checks if the variable name can be found in envp_at_idx_i.   */
/* Two cases are check : if the char at index var_name_len - 1 is or is not a */
/* "=". This will define how many char are compared for the checks. Then we   */
/* check if the first char after the compared part in envp_at_idx_i is a "="  */
/* or a "\0".                                                                 */
/* If var_name is found at the beginning of envp_at_idx_i and that a "=" or   */
/* "\0" follows then var_name is found in envp_at_idx_i and 1 is return       */
/* Inputs :                                                                   */
/*  - char *envp_at_idx_i : a string that contained datas envp at index i     */
/*  - char *var_name : the string that we try to find                         */
/*  - size_t var_name_len : the lenght of the string var_name                 */
/* Return :                                                                   */
/*  - 1 : if var_name is found in envp_at_idx_i                               */
/*  - 0 : otherwise                                                           */
/* ************************************************************************** */
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

/* ************************************************************************** */
/*                                 copy_envp                                  */
/* -------------------------------------------------------------------------- */
/* This function copies the i first strings of envp in a NULL terminated      */
/* array of strings tab. This array will then replace envp variable and       */
/* envp_size will be update                                                   */
/* rq : if i == envp_size then a void string is add at the end of envp which  */
/* size will be increased by one                                              */
/* Inputs :                                                                   */
/*  - char ***envp : a pointer to the array of string to copy (i firrst elt)  */
/*  - int *envp_size : a pointer to the size of the array of strings envp     */
/*  - int i : the number of elements to copy from envp                        */
/* Return :                                                                   */
/*  - 0 : if everything goes well                                             */
/*  - int : the error code of the problem encounter (-3 malloc error)         */
/* ************************************************************************** */
// why don't we do tab[i] = NULL to be sure that the allocate memory space doesn't
// contained random datas ???
// Let's be more specific :
// tab contained i + 2 strings
// we copy envp into tab element into while loop
// we do so until j < i && j < envp_size
// which mean that tab[0:j] contained something (we disregard malloc error)
// (for j == (i - 1) || j == (envp_size - 1))
//
// let's use some random numerical value :
// *** we suppose that i = 3 && env_size = 8
// => tab = [[],[],[],[],[]]
// loop => tab = [envp[0], envp[1], envp[2], [], []]
// then we do tab[i + 1] = NULL => tab = [envp[0], envp[1], envp[2], [], NULL]
//
// *** we suppose that i == envp_size  == 3 ***
// the reasoning is the same as previously
//
// *** now, let's suppose i = 8 && envp_size = 3 *** (worse case)
// => tab = [[],[],[],[],[],[],[],[],[],[]]
// loop => tab = [envp[0], envp[1], envp[2], [], [], [], [], [], [], []]
// then we do tab[i + 1] = NULL => tab = [envp[0], envp[1], envp[2], [], [], [], [], [], [], NULL]
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

/* ************************************************************************** */
/*                            update_envp_with_var                            */
/* -------------------------------------------------------------------------- */
/* This function updates the envp. It the index "i" received is smaller than  */
/* envp_size then envp[i] is free and replace by var                          */
/* Otherwise, if i == envp_size then envp is expend to be able to add var at  */
/* the end of envp                                                            */
/* Inputs :                                                                   */
/*  - char ***envp : a pointer to an array of string about the environment    */
/*  - int *envp_size : a pointer to the size of the array of strings envp     */
/*  - char *var : the string with which we want to replace envp[i]            */
/*  - int i : the index of the element to replace                             */
/* Return :                                                                   */
/*  - 0 : if everything goes well                                             */
/*  - int : the error code of the problem encounter                           */
/* ************************************************************************** */
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
