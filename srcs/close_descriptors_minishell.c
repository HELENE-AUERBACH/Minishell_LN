/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close_descriptors_minishell.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 17:46:38 by hauerbac          #+#    #+#             */
/*   Updated: 2024/05/21 16:53:11 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	perror_and_code(int *result, const char *s, int new_result)
{
	if (*result == 0)
		*result = new_result;
	perror(s);
}

int	close_descriptors(int pipedescr[3], int is_piped)
{
	int	result;

	result = 0;
	if (is_piped && pipedescr[0] != -1 && close(pipedescr[0]) != 0)
		perror_and_code(&result, CLOSE_0_ERR, -1);
	pipedescr[0] = -1;
	if (is_piped && pipedescr[1] != -1 && close(pipedescr[1]) != 0)
		perror_and_code(&result, CLOSE_1_ERR, -2);
	pipedescr[1] = -1;
	if (pipedescr[2] != -1 && close(pipedescr[2]) != 0)
		perror_and_code(&result, CLOSE_PREV_ERR, -3);
	pipedescr[2] = -1;
	return (result);
}

int	perr_cds(t_data *d, const char *s, int pipedescriptors[3], int is_piped)
{
	perror(s);
	close_descriptors(pipedescriptors, is_piped);
	empty_list(&d->cmds);
	empty_list(&d->new_files);
	empty_list(&d->cmd_new_files);
	if (is_in_interactive_mode())
		rl_clear_history();
	free_data(d);
	if (s && ft_strncmp("Exec command failed error", s, 25) == 0
		&& errno == EACCES)
		exit (126);
	exit (EXIT_FAILURE);
}

void	close_descrs_with_a_possible_exit(t_data *d, t_token *t,
		int pipedescr[3], int is_piped)
{
	int	result;

	result = 0;
	if (t && t->cmd_d && t->cmd_d->fd1 != -1)
		close(t->cmd_d->fd1);
	t->cmd_d->fd1 = -1;
	if (t && t->cmd_d && t->cmd_d->fd2 != -1)
		close(t->cmd_d->fd2);
	t->cmd_d->fd2 = -1;
	if (t && close_descriptors(pipedescr, is_piped) != 0)
	{
		empty_list(&d->cmds);
		empty_list(&d->new_files);
		empty_list(&d->cmd_new_files);
		if (is_in_interactive_mode())
			rl_clear_history();
		free_data(d);
		exit (EXIT_FAILURE);
	}
	empty_list(&d->new_files);
	empty_list(&d->cmd_new_files);
	if (t->src)
		free(t->src);
	t->src = NULL;
}

void	close_ds_in_parent(t_token *t, int ds[3], int is_piped)
{
	int	result;

	result = 0;
	if (t && t->cmd_d && t->cmd_d->fd1 != -1 && close(t->cmd_d->fd1) == -1)
		perror(PARENT_CLOSE_IN_ERR);
	else if (t && t->cmd_d)
		t->cmd_d->fd1 = -1;
	if (ds[2] != -1 && close(ds[2]) != 0)
		perror(PARENT_CLOSE_PREV_ERR);
	ds[2] = -1;
	if (is_piped && ds[1] != -1 && close(ds[1]) != 0)
		perror(PARENT_CLOSE_1_ERR);
	ds[1] = -1;
	if (t && t->cmd_d && t->cmd_d->fd2 != -1 && close(t->cmd_d->fd2) == -1)
		perror(PARENT_CLOSE_OUT_ERR);
	else if (t && t->cmd_d)
		t->cmd_d->fd2 = -1;
}
