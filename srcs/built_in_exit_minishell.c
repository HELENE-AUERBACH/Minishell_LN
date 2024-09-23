/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_exit_minishell.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmorice <rmorice@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 13:34:39 by hauerbac          #+#    #+#             */
/*   Updated: 2024/09/23 11:22:08 by rmorice          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* ************************************************************************** */
/*                               too_many_args                                */
/* -------------------------------------------------------------------------- */
/* This function checks the number of arguments (size of args)                */
/* This function is call for exit builtin that doesn't accept more than 2     */
/* arguments                                                                  */
/* Input :                                                                    */
/*  - char **args : an array of char that contained arguments (check is size) */
/* Return :                                                                   */
/*  - 1 : if the number of arguments is greater than 2 (too many arguments)   */
/*  - 0 : otherwise                                                           */
/* ************************************************************************** */
static int	too_many_args(char **args)
{
	int	i;

	i = 1;
	while (args[i])
	{
		i++;
	}
	if (i > 2)
		return (1);
	return (0);
}

/* ************************************************************************** */
/*                                 get_value                                  */
/* -------------------------------------------------------------------------- */
/* This function converts exit_value into a long long int in base 10          */
/* if an error occured then an error message is display, otherwise the signed */
/* value obtained is returned                                                 */
/* rq : if an error occured, error value become 1                             */
/* Inputs :                                                                   */
/*  - int *error : a pointer to an int used to specified if an error occured  */
/*  - char *exit_value : a string that contained the exit value to return     */
/* Return :                                                                   */
/*  - long long : the value obtained in base 10 (if everything goes well)     */
/*  - 2 : if an error occured                                                 */
/* ************************************************************************** */
static long long	get_value(int *error, char *exit_value)
{
	int					sign;
	char				*endptr;
	unsigned long long	uval;

	sign = 0;
	uval = ft_strtoll(exit_value, &endptr, &sign);
	if (endptr && (endptr == exit_value || endptr[0] != '\0'))
	{
		display_err_with_prefix(exit_value, \
					" numeric argument required\n");
		*error = 1;
		return (2);
	}
	return (uval * sign);
}

/* ************************************************************************** */
/*                                 built_exit                                 */
/* -------------------------------------------------------------------------- */
/* This function terminates execution with STATUS {value % 256}               */
/* This cause the shell or the subshell to exit its current execution         */
/* environment                                                                */
/* rq if we are in a subshell then the shell exit the subshell and continue   */
/* in the environment from which the subshell was invoked                     */
/* rq : if we are in interactive mode the history is clear beforre exit       */
/* Inputs :                                                                   */
/*  - t_data *d : a structure that contained infos relative to the shell      */
/*  - char **args : array that contained the arguments associated to builtin  */
/* Return :                                                                   */
/*  - int : the exit status obtained (value % 256)                            */
/* ************************************************************************** */
int	built_exit(t_data *d, char **args)
{
	long long	value;
	int			error;

	if (!args[1])
		value = d->return_code;
	else
	{
		error = 0;
		value = get_value(&error, args[1]);
		if (!error && too_many_args(args))
			return (display_err_with_prefix(args[0], \
						" too many arguments\n"), 1);
	}
	empty_list(&d->cmds);
	empty_list(&d->new_files);
	empty_list(&d->cmd_new_files);
	if (is_in_interactive_mode())
		rl_clear_history();
	free_data(d);
	exit (value % 256);
}
