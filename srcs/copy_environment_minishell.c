/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_environment_minishell.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmorice <rmorice@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 18:51:27 by hauerbac          #+#    #+#             */
/*   Updated: 2024/09/12 14:14:21 by rmorice          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* ************************************************************************** */
/*                            get_environment_size                            */
/* -------------------------------------------------------------------------- */
/* This function calculates the size of the char array that contained the     */
/* informations relative to the system environment                            */
/* Input :                                                                    */
/*  - char **envp : the system environment datas                              */
/* Return :                                                                   */
/*  - 0 : if envp is NULL or void                                             */
/*  - int : othewrwise                                                        */
/* ************************************************************************** */
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

/* ************************************************************************** */
/*                            get_new_shell_level                             */
/* -------------------------------------------------------------------------- */
/* This function calculates the "new" shlvl. It increment by one the current  */
/* shell level (if no information about the shell level are known it          */
/* initialises the shlvl to 1).                                               */
/* Input :                                                                    */
/*  - char **envp : the system environment datas                              */
/* Return :                                                                   */
/*  - char * : a char formated "SHLVL="+int                                   */
/*  - and if a problem occured ???                                            */
/* ************************************************************************** */
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

/* ************************************************************************** */
/*                        exit_when_error_on_envp_copy                        */
/* -------------------------------------------------------------------------- */
/* This function frees the array that contained a copy of infomations about   */
/* the environment (and every elements until an error in the copy occured).   */
/* Then it displays an error message and exits with the code error 1.         */
/* Inputs :                                                                   */
/*  - t_data *d : the structures that contained the array to free             */
/*  - int j : the index of the element of the array where the error occured   */
/* Return :                                                                   */
/*  - None                                                                    */
/* ************************************************************************** */
static void	exit_when_error_on_envp_copy(t_data *d, int j)
{
	while (j > 0 && d->envp[j - 1])
		free(d->envp[--j]);
	free(d->envp);
	d->envp = NULL;
	display_error("Environment copy error\n");
	exit (1);
}

/* ************************************************************************** */
/*                          copy_environment_strings                          */
/* -------------------------------------------------------------------------- */
/* This function copies the environment datas into an array contained into    */
/* the structure d. The line relative to SHLVL is replace by the array shlvl  */
/* Inputs :                                                                   */
/*  - t_data *d : the structures that contained the array to complete         */
/*  - char **envp : the system environment datas                              */
/*  - char *shlvl : string well formated about the actual SHLVL               */
/* Return :                                                                   */
/*  - None                                                                    */
/* ************************************************************************** */
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

/* ************************************************************************** */
/*                              copy_environment                              */
/* -------------------------------------------------------------------------- */
/* This function copies the environment. It defines as well the size of the   */
/* environment, the shell level and add the cwd (current woking directory) to */
/* the t_data struct. If their is no environment then it specified the cdw    */
/* and the shelvl.                                                            */
/* Inputs :                                                                   */
/*  - t_data *d : a structure that contained datas about our shell            */
/*  - char **envp : the system environment datas                              */
/* Return :                                                                   */
/*  - None                                                                    */
/* ************************************************************************** */
void	copy_environment(t_data *d, char **envp)
{
	int		to_be_copied;
	char	*shlvl_new;

	d->envp_size = get_environment_size(envp);
	d->envp = NULL;
	to_be_copied = 0;
	shlvl_new = get_new_shell_level();
	if (d->envp_size == 0)
		d->envp_size = 2;
	else
		to_be_copied = 1;
	d->envp = (char **) malloc((d->envp_size + 1) * sizeof(char *));
	if (d->envp && to_be_copied)
		copy_environment_strings(d, envp, shlvl_new);
	else if (d->envp && !to_be_copied)
	{
		copy_pwd_var_into_env(d->envp, d->envp_size);
		d->envp[1] = (char *) malloc((7 + 1) * sizeof(char));
		if (!d->envp[0] || !d->envp[1])
			exit_when_error_on_envp_copy(d, 1);
		ft_strlcpy(d->envp[1], shlvl_new, 7 + 1);
	}
	if (shlvl_new)
		free(shlvl_new);
	return ;
}
