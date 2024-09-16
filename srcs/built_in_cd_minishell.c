/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_cd_minishell.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmorice <rmorice@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 16:57:29 by hauerbac          #+#    #+#             */
/*   Updated: 2024/09/12 13:36:27 by rmorice          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* ************************************************************************** */
/*                       update_oldpwd_pwd_vars_in_envp                       */
/* -------------------------------------------------------------------------- */
/* This function updates the value associated to OLDPWD with previous_pwd and */
/* PWD with the pathname of the current working directory                     */
/* Inputs :                                                                   */
/*  - char ***envp : a pointer to an array of string about the environment    */
/*  - int *envp_size : the size of the array of strings envp                  */
/*  - char **previous_pwd : a pointer to a string that contained previous pwd */
/* Return :                                                                   */
/*  - 0 : if everything goes well                                             */
/*  - int : the error code of the problem encounter                           */
/* ************************************************************************** */
static int	update_oldpwd_pwd_vars_in_envp(char ***envp, int *envp_size,
			char **previous_pwd)
{
	int		result;
	char	*new_dir_val;

	result = updenv(envp, envp_size, "OLDPWD=", *previous_pwd);
	if (result == 0)
	{
		new_dir_val = getcwd(NULL, 0);
		if (!new_dir_val)
		{
			display_err_with_prefix("cd", \
				" error retrieving current directory\n");
			result = 1;
		}
		else
		{
			result = updenv(envp, envp_size, "PWD=", new_dir_val);
			free(new_dir_val);
		}
	}
	if (previous_pwd && *previous_pwd)
		free(*previous_pwd);
	*previous_pwd = NULL;
	return (result);
}

/* ************************************************************************** */
/*                              change_directory                              */
/* -------------------------------------------------------------------------- */
/* This function changes the current working directory for the one associated */
/* with the new_dir variable in envp.                                         */
/* If no such value is found then an error message is display on the error    */
/* standard output                                                            */
/* Otherwise the process's working directory is changed to the value obtained */
/* If an error occured in this process then an error message is display       */
/* Inputs :                                                                   */
/*  - char *new_dir : the path to the new directory where we want to "work"   */
/*  - char ***envp : a pointer to an array of string about the environment    */
/*  - int *envp_size : a pointer to the size of the array of strings envp     */
/* Return :                                                                   */
/*  - 0 : if everything goes well                                             */
/*  - int : the error code of the problem encounter                           */
/* ************************************************************************** */
static int	change_directory(char *new_dir, char **envp, int envp_size)
{
	char	*new_dir_val;

	if (get_value_from_envp(&new_dir_val, new_dir, envp_size, envp) == 3)
		return (-3);
	if (!new_dir_val)
	{
		write(2, "cd: ", 4);
		write(2, new_dir, ft_strlen(new_dir));
		write(2, " not set\n", 9);
		return (-1);
	}
	else if (chdir(new_dir_val) == -1)
		return (write(2, "cd: ", 4), perror(new_dir_val), \
			free(new_dir_val), -2);
	return (free(new_dir_val), 0);
}

/* ************************************************************************** */
/*                                 execute_cd                                 */
/* -------------------------------------------------------------------------- */
/* This function changes the current working directory and updates the envp   */
/* datas associated (PWD, OLD_PWD).                                           */
/* rq : "cd -" => go back to the previous directory                           */
/*      "cd" => go back to home                                               */
/* Inputs :                                                                   */
/*  - char ***envp : a pointer to an array of string about the environment    */
/*  - int *envp_size : a pointer to the size of the array of strings envp     */
/*  - char **args      */
/*  - int i :       */
/* Return :                                                                   */
/*  - 0 : if everything goes well                                             */
/*  - int : the error code of the problem encounter                           */
/* ************************************************************************** */
static int	execute_cd(char ***envp, int *envp_size, char **args, int i)
{
	char	*cwd;
	char	*previous_pwd;
	int		result;

	cwd = getcwd(NULL, 0);
	if (!cwd && i == 2 && access(args[1], F_OK))
		return (display_err_with_2_prefixes(WRONG_CWD_1, WRONG_CWD_2, \
			" No such file or directory\n"), perror(args[0]), 1);
	if (get_value_from_envp(&previous_pwd, "PWD", *envp_size, *envp) == 3)
		return (free(cwd), -3);
	if (i == 1)
		result = change_directory("HOME", *envp, *envp_size);
	else if (i == 2 && args[1][0] == '-')
		result = change_directory("OLDPWD", *envp, *envp_size);
	if (i == 1 || (i == 2 && args[1][0] == '-'))
	{
		if (result == -3)
			return (free(previous_pwd), free(cwd), -3);
		else if (result < 0)
			return (free(previous_pwd), free(cwd), 1);
	}
	else if (i == 2 && args[1][0] != '-' && chdir(args[1]) == -1)
		return (perror(args[1]), free(previous_pwd), free(cwd), 1);
	return (free(cwd), update_oldpwd_pwd_vars_in_envp(envp, envp_size, \
		&previous_pwd));
}

/* ************************************************************************** */
/*                                  built_cd                                  */
/* -------------------------------------------------------------------------- */
/* This function checks if args size is inferior or equal to 2 (cd and one    */
/* option of type "-" or path). Then, if args is well formatted, it changes   */
/* the current working directory and updates the envp  datas associated (PWD, */
/* OLD_PWD)                                                                   */
/* rq : "cd -" => go back to the previous directory                           */
/*      "cd" => go back to home                                               */
/* rq2 : if the opt is "-" then new_dir_var (new working dir) is write in fd  */
/* Inputs :                                                                   */
/*  - char ***envp : a pointer to an array of string about the environment    */
/*  - int *envp_size : a pointer to the size of the array of strings envp     */
/*  - char **args      */
/*  - int fd : the output file descriptor                                     */
/* Return :                                                                   */
/*  - 0 : if everything goes well                                             */
/*  - int : the error code of the problem encounter                           */
/* ************************************************************************** */
int	built_cd(char ***envp, int *envp_size, char **args, int fd)
{
	int		i;
	int		result;
	char	*new_dir_val;

	if (!(envp && *envp))
		return (-1);
	i = 0;
	while (args[i])
		i++;
	if (i > 2)
		return (display_err_with_prefix("cd", " too many arguments\n"), 1);
	else if (i == 2 && ft_strlen(args[1]) > 1 && args[1][0] == '-')
		return (display_err_with_2_prefixes(args[0], args[1], \
						" invalid option\n"), 2);
	result = execute_cd(envp, envp_size, args, i);
	new_dir_val = getcwd(NULL, 0);
	if (result == 0 && i == 2 && args[1][0] == '-' && new_dir_val)
	{
		write(fd, new_dir_val, ft_strlen(new_dir_val));
		write(fd, "\n", 1);
	}
	if (result == 0 && i == 2 && args[1][0] == '-' && !new_dir_val)
		return (display_err_with_prefix(args[0], \
				" error retrieving current directory\n"), 1);
	return (free(new_dir_val), result);
}
