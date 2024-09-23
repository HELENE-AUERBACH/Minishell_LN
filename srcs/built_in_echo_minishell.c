/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_echo_minishell.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmorice <rmorice@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 11:32:57 by hauerbac          #+#    #+#             */
/*   Updated: 2024/09/23 11:17:41 by rmorice          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* ************************************************************************** */
/*                         check_options_and_newline                          */
/* -------------------------------------------------------------------------- */
/* This function checks if we have to apply the "-n" option. This option is   */
/* trigger as long as the 2 first char are "-n" and all the following char of */
/* the string until "\0" are 'n'.                                             */
/* The index of the args element checked is incremented until we are no       */
/* longer into a "-n" option case o all args had been review                  */
/* If that is the case then the variable with_a_newline is set as 0 (the      */
/* trailing newline will not be display). Otherwise it is set to 1.           */
/* Inputs :                                                                   */
/*  - int *i : a pointer to the first index of args to check                  */
/*  - int *with_a_newline : pointer to an int used to display or not newline  */
/*  - char **args : array that contained the arguments associated to builtin  */
/* Return :                                                                   */
/*  - None                                                                    */
/* ************************************************************************** */
static void	check_options_and_newline(int *i, int *with_a_newline,
			char **args)
{
	size_t	j;

	while (args[*i] && ft_strlen(args[*i]) > 0 \
			&& ft_strncmp("-n", args[*i], 2) == 0)
	{
		*with_a_newline = 0;
		j = 2;
		while (j < ft_strlen(args[*i]))
		{
			if (args[*i][j] != 'n')
			{
				*with_a_newline = 1;
				return ;
			}
			j++;
		}
		(*i)++;
	}
}

/* ************************************************************************** */
/*                                 built_echo                                 */
/* -------------------------------------------------------------------------- */
/* This function checks if the firsts options are well formatted "-n" type.   */
/* If that is the case then the following args will be write in fd output and */
/* the trailing newline is not write. Otherwise, starting from the first args */
/* that isn't a well formatted "-n" (or not a "-n" option at all) every       */
/* remaining args are write in the output fd and the trailing newline is      */
/* write (newline is write once all the remaining args had been write)        */
/* rq : every args to write is separated by a space                           */
/* Inputs :                                                                   */
/*  - char **args : array that contained the arguments associated to builtin  */
/*  - int fd : the output file descriptor                                     */
/* Return :                                                                   */
/*  - 0                                                                       */
/* ************************************************************************** */
int	built_echo(char **args, int fd)
{
	int	i;
	int	with_a_newline;

	i = 1;
	with_a_newline = 1;
	check_options_and_newline(&i, &with_a_newline, args);
	while (args[i] && ft_strlen(args[i]) > 0)
	{
		write(fd, args[i], ft_strlen(args[i]));
		i++;
		if (args[i] && ft_strlen(args[i]) > 0)
			write(fd, " ", 1);
	}
	if (with_a_newline)
		write(fd, "\n", 1);
	return (0);
}
