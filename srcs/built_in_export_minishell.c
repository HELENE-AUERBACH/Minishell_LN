/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_export_minishell.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmorice <rmorice@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 15:08:25 by hauerbac          #+#    #+#             */
/*   Updated: 2024/09/24 12:06:37 by rmorice          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* ************************************************************************** */
/*                               print_env_var                                */
/* -------------------------------------------------------------------------- */
/* This function writes into fd the environment variable given in input while */
/* following the format "export "<var_name>"=\""<var_value>"\"\n" or, if no   */
/* "=" char is met, "export "<var_name>"\n"                                   */
/* rq : if env_var == "_=env" nothing is display                              */
/* Inputs :                                                                   */
/*  - char *env_var : the environment variable to display                     */
/*  - size_t env_var_len : the length of the environment variable             */
/*  - int fd : the output file descriptor                                     */
/* Return :                                                                   */
/*  - None                                                                    */
/* ************************************************************************** */
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

/* ************************************************************************** */
/*                          split_var_name_and_value                          */
/* -------------------------------------------------------------------------- */
/* This function splits the args[i] into a variable name and its associated   */
/* value. This two parts are copy respectively into vns and vvals             */
/* Before the copy we check if the var_name is well formatted (only alphanum  */
/* char or "_") until the "=" char that delimit the two part to split or "\0" */
/* Inputs :                                                                   */
/*  - int i : the index of the element of args that we want to split          */
/*  - char **args : an array of str that containe variable name and its value */
/*  - char **vns : an array that will contained the variable name found       */
/*  - char **vvals : an array that will contained the variable value found    */
/* Return :                                                                   */
/*  - 0 : if everything goes well                                             */
/*  - int : the error code of the problem encounter                           */
/* ************************************************************************** */
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

/* ************************************************************************** */
/*                                   updenv                                   */
/* -------------------------------------------------------------------------- */
/* This function updates the environment datas to add or update the value     */
/* associated to var_name. To do so we check if var_name already exist in     */
/* envp and, if that is the case what is the index associated                 */
/* If var_name exist then var_value is update, otherwise var_name and its     */
/* associated value are add at the end of envp (which size increased by one)  */
/* Inputs :                                                                   */
/*  - char ***envp : a pointer to an array of string about the environment    */
/*  - int *envp_size : a pointer to the size of the array of strings envp     */
/*  - char *var_name : the name of the variable that we want to update or add */
/*  - char *var_value : the current value associated to var_name              */
/* Return :                                                                   */
/*  - 0 : if everything goes well                                             */
/*  - int : the error code of the problem encounter                           */
/* ************************************************************************** */
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

/* ************************************************************************** */
/*                                export_vars                                 */
/* -------------------------------------------------------------------------- */
/* This function splits the args[i] into a variable name and its associated   */
/* value for the first nv elements of args.                                   */
/* envp is update to keep val_name and its associated value                   */
/* Inputs :                                                                   */
/*  - char ***envp : a pointer to an array of string about the environment    */
/*  - int *envp_size : a pointer to the size of the array of strings envp     */
/*  - char **args : array of options and arguments of the builtin             */
/*  - int nv : the number of variables to splits                              */
/* Return :                                                                   */
/*  - 0 : if everything goes well                                             */
/*  - int : the error code of the problem encounter                           */
/* ************************************************************************** */
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

/* ************************************************************************** */
/*                                built_export                                */
/* -------------------------------------------------------------------------- */
/* This function exports variables into envp if size args[i] > 1 or, writes   */
/* into fd the environment variable if size args[i] == 1                      */
/* rq : if OLDPWD isn't found into envp then a default export message is      */
/* write into fd                                                              */
/* Inputs :                                                                   */
/*  - char ***envp : a pointer to an array of string about the environment    */
/*  - int *envp_size : a pointer to the size of the array of strings envp     */
/*  - char **args : array that contained the arguments associated to builtin  */
/*  - int fd : the output file descriptor                                     */
/* Return :                                                                   */
/*  - 0 : if everything goes well                                             */
/*  - int : the error code of the problem encounter                           */
/* ************************************************************************** */
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
