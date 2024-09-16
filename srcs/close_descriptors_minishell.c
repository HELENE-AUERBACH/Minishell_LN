/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close_descriptors_minishell.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmorice <rmorice@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 17:46:38 by hauerbac          #+#    #+#             */
/*   Updated: 2024/09/12 13:18:36 by rmorice          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* ************************************************************************** */
/*                              perror_and_code                               */
/* -------------------------------------------------------------------------- */
/* This function display a message error associated to the value of errno and */
/* update, if needed, the result value (overall error code)                   */
/* Inputs :                                                                   */
/*  - int *result : a pointer to an error code that might need to be update   */
/*  - const char *s : errno value ??? */
/*  - int new_result : the value by which result might be replace             */
/* Return :                                                                   */
/*  - None                                                                    */
/* ************************************************************************** */
void	perror_and_code(int *result, const char *s, int new_result)
{
	if (*result == 0)
		*result = new_result;
	perror(s);
}

/* ************************************************************************** */
/*                             close_descriptors                              */
/* -------------------------------------------------------------------------- */
/* This function closes pipe's descriptors and pipedescr values are set to -1 */
/* If an error occured then an error message is display and result value is   */
/* update to specified where the error occured.                               */
/* Inputs :                                                                   */
/*  - int pipedescr[3]      */
/*  - int *is_piped      */
/* Return :                                                                   */
/*  - 0 : if everything goes well                                             */
/*  - int : the error code of the problem encounter                           */
/* ************************************************************************** */
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

/* ************************************************************************** */
/*                                  perr_cds                                  */
/* -------------------------------------------------------------------------- */
/* This function displays an error message, closes the pipe's descriptors and */
/* free every datas about commands and files relative to the current state    */
/* If we are in interactive mode then the history is cleared                  */
/* Then we exit the system with EXIT_FAILURE value if everything goes well    */
/* and 126 if a specific error occured                                        */
/* Inputs :                                                                   */
/*  - t_data *d : a structure that contained infos relative to the shell      */
/*  - const char *s : the error message to display                            */
/*  - int pipedescriptors[3]      */
/*  - int is_piped      */
/* Return :                                                                   */
/*  - int EXIT_FAILURE : if everything goes well                              */
/*  - 126 : if the command execution failed and errno equal EACCES            */
/* ************************************************************************** */
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

/* ************************************************************************** */
/*                     close_descrs_with_a_possible_exit                      */
/* -------------------------------------------------------------------------- */
/* This function closes file's descriptors as well as pipe's descriptor and   */
/* set fd1, fd2 and pipedescr values to -1                                    */
/* It also frees every datas about commands and files relative to the current */
/* state. If we are in interactive mode then the history is cleared           */
/* rq : if an error occured while closing pipe's descriptor then the function */
/* is exit with EXIT_FAILURE value, otherwise src is free                     */
/* Inputs :                                                                   */
/*  - t_data *d : a structure that contained infos relative to the shell      */
/*  - t_token *t     */
/*  - int pipedescr[3]      */
/*  - int is_piped      */
/* Return :                                                                   */
/*  - None                                                                    */
/* ************************************************************************** */
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

/* ************************************************************************** */
/*                             close_ds_in_parent                             */
/* -------------------------------------------------------------------------- */
/* This function closes file descriptors and ??? (ds). If an error occured an */
/* error message is display                                                   */
/* If an element as been closed without error then it value is set to -1      */
/* Inputs :                                                                   */
/*  - t_token *t     */
/*  - int ds[3]      */
/*  - int is_piped      */
/* Return :                                                                   */
/*  - None                                                                    */
/* ************************************************************************** */
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
