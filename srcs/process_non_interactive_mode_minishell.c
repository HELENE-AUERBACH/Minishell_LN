/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_non_interactive_mode_minishell.c           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmorice <rmorice@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 17:39:18 by hauerbac          #+#    #+#             */
/*   Updated: 2024/09/26 13:02:35 by rmorice          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* ************************************************************************** */
/*                                   loop1                                    */
/* -------------------------------------------------------------------------- */
/* This function reads BUFFER_SIZE characters from the fd input and puts them */
/* into char *buff.                                                           */
/* rq : if the read part end with at least one '\n' then these char are       */
/* replace by '\0'                                                            */
/* Inputs :                                                                   */
/*  - int fd : the file descriptor relative to the input                      */
/*  - char *buff : a string type buffer that contained what is read           */
/* Return :                                                                   */
/*  - int : the lenght of the line readed (max BUFFER_SIZE)                   */
/* ************************************************************************** */
static ssize_t	loop1(int fd, char *buff)
{
	ssize_t		read_len;
	ssize_t		i;

	read_len = read(fd, buff, BUFFER_SIZE);
	if (read_len != -1)
	{
		i = BUFFER_SIZE;
		while (i >= read_len
			|| (i < read_len && i >= 0 && buff[i] == '\n'))
			buff[i--] = '\0';
	}
	return (read_len);
}

/* ************************************************************************** */
/*                                   loop3                                    */
/* -------------------------------------------------------------------------- */
/* This function process the line (splits it into tokens, determines the role */
/* of each token, do redirections and extension and runs builtin functions or */
/* sub-set of commands)                                                       */
/* Then it frees and point toward NULL everything that is no longer usefull   */
/* Inputs :                                                                   */
/*  - t_data *d : the structure that contained infos relative to the shell    */
/*  - ssize_t read_len : the length of the string read (nb char read)         */
/*  - char *buff : a string type buffer that contained what is read           */
/*  - ssize_t j : the index of the beginning of the line to process           */
/* Return :                                                                   */
/*  - 0 : if everything goes well                                             */
/*  - int : the error code of the problem encounter                           */
/* ************************************************************************** */
static int	loop3(t_data *d, ssize_t read_len, char *buff, ssize_t j)
{
	int	result;

	result = 0;
	if (read_len > 1)
	{
		result = process_a_line(buff + j, d);
		empty_list(&d->cmds);
		empty_list(&d->new_files);
		empty_list(&d->cmd_new_files);
		empty_dll(d->lst, del_el_content);
	}
	return (result);
}

/* ************************************************************************** */
/*                                   loop2                                    */
/* -------------------------------------------------------------------------- */
/* This function process every lines that has been encountered.               */
/* To do so, it splits the line into tokens, determines the role of each      */
/* token, do redirections and extension and runs builtin functions or sub-set */
/* of commands                                                                */
/* rq : the command line to process is either what preceded a '\n', what is   */
/* contained between two '\n', what end with a '\0'. Newline is the end of    */
/* one line to process and what preceded the start of the next line to        */
/* process                                                                    */
/* Inputs :                                                                   */
/*  - t_data *d : the structure that contained infos relative to the shell    */
/*  - ssize_t read_len : the length of the string read (nb char read)         */
/*  - char *buff : a string type buffer that contained what is read           */
/* Return :                                                                   */
/*  - 0 : if everything goes well                                             */
/*  - int : the error code of the problem encounter                           */
/* ************************************************************************** */
static int	loop2(t_data *d, ssize_t read_len, char *buff)
{
	ssize_t		i;
	ssize_t		j;
	int			result;

	i = 0;
	j = 0;
	while (i < read_len)
	{
		if (buff[i] == '\n')
		{
			buff[i] = '\0';
			result = loop3(d, read_len, buff, j);
			if (result != 0)
				return (result);
			j = i + 1;
		}
		i++;
	}
	if (buff[j] != '\0')
	{
		result = loop3(d, read_len, buff, j);
		if (result != 0)
			return (result);
	}
	return (0);
}

/* ************************************************************************** */
/*                        process_non_interactive_mode                        */
/* -------------------------------------------------------------------------- */
/* This function initialises the data relatives to the shell status. Then it  */
/* reads the input and process it line by line                                */
/* Inputs :                                                                   */
/*  - int fd : the file descriptor relative to the input                      */
/*  - t_data *d : the structure that contained infos relative to the shell    */
/* Return :                                                                   */
/*  - 0 : if everything goes well                                             */
/*  - int : the error code of the problem encounter                           */
/* ************************************************************************** */
int	process_non_interactive_mode(int fd, t_data *d)
{
	ssize_t	read_len;
	char	buff[BUFFER_SIZE + 1];
	int		result;

	if (!d)
		return (EXIT_FAILURE);
	d->lst = new_empty_dll();
	if (!d->lst)
		return (EXIT_FAILURE);
	d->cmds = NULL;
	d->nb_cmds = 0;
	d->return_code = 0;
	d->new_files = NULL;
	d->cmd_new_files = NULL;
	read_len = 1;
	while (read_len > 0)
	{
		read_len = loop1(fd, buff);
		if (read_len == -1)
			return (perror("read error"), free_data(d), 1);
		result = loop2(d, read_len, buff);
		if (result != 0)
			return (free_data(d), result);
	}
	return (free_data(d), 0);
}

/* ************************************************************************** */
/*                  process_non_interactive_mode_with_c_opt                   */
/* -------------------------------------------------------------------------- */
/* This function process the input obtained through a non interactive mode    */
/* with 'c' option                                                            */
/* The option c mean that we will processed an unique command (3rd input of   */
/* the minishell script). This command should be contained into double quoted */
/* or be only one string                                                      */
/* Inputs :                                                                   */
/*  - char *an_argv : the argument to process (the command line to execute)   */
/*  - t_data *d : the structure that contained infos relative to the shell    */
/* Return :                                                                   */
/*  - 0 : if everything goes well                                             */
/*  - int : the error code of the problem encounter                           */
/* ************************************************************************** */
int	process_non_interactive_mode_with_c_opt(char *an_argv, t_data *d)
{
	int	result;

	if (!d)
		return (EXIT_FAILURE);
	d->lst = new_empty_dll();
	if (!d->lst)
		return (EXIT_FAILURE);
	d->cmds = NULL;
	d->nb_cmds = 0;
	d->return_code = 0;
	d->new_files = NULL;
	d->cmd_new_files = NULL;
	result = process_a_line(an_argv, d);
	empty_list(&d->cmds);
	empty_list(&d->new_files);
	empty_list(&d->cmd_new_files);
	if (result != 0)
		return (free_data(d), result);
	return (free_data(d), 0);
}
