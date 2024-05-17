/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_non_interactive_mode_minishell.c           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 17:39:18 by hauerbac          #+#    #+#             */
/*   Updated: 2024/05/07 19:23:56 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static int	loop3(t_data *d, ssize_t read_len, char *buff, ssize_t j)
{
	int	result;

	result = 0;
	if (read_len > 1)
	{
		result = process_a_line(buff + j, d);
		print_dll(d->lst, 2, print_dll_el_content);
		empty_list(&d->cmds);
		empty_list(&d->new_files);
		empty_list(&d->cmd_new_files);
		empty_dll(d->lst, del_el_content);
	}
	return (result);
}

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
	d->new_files = NULL;
	d->cmd_new_files = NULL;
	result = process_a_line(an_argv, d);
	print_dll(d->lst, 2, print_dll_el_content);
	empty_list(&d->cmds);
	empty_list(&d->new_files);
	empty_list(&d->cmd_new_files);
	if (result != 0)
		return (free_data(d), result);
	return (free_data(d), 0);
}
