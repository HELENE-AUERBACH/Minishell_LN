/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks_split_lexer_minishell.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmorice <rmorice@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 12:28:43 by hauerbac          #+#    #+#             */
/*   Updated: 2024/08/19 14:29:17 by rmorice          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_minishell.h"

/* ************************************************************************** */
/*                            check_double_quotes                             */
/* -------------------------------------------------------------------------- */
/* This function updates the double quoted status if needed.                  */
/* If the char is a double quote and the char is outside a single quote then  */
/* if no lonely double quote exists then a new quoted part begin, otherwise   */
/* the quoted part arrived to an end                                          */
/* Inputs :                                                                   */
/*  - int i : the index to check                                              */
/*  - int *data : array that contained infos about the current situation      */
/*  - const char *str : the command line to check                             */
/* Return :                                                                   */
/*  - None                                                                    */
/* ************************************************************************** */
void	check_double_quotes(int i, int *data, const char *str)
{
	if (str[i] == '"' && data[EXT_OPEN_S_QUOTE] == 0)
	{
		if (data[EXT_OPEN_D_QUOTE] == 0)
			data[EXT_OPEN_D_QUOTE] = 1;
		else if (data[EXT_OPEN_D_QUOTE] == 1)
			data[EXT_OPEN_D_QUOTE] = 0;
	}
}

/* ************************************************************************** */
/*                            check_simple_quotes                             */
/* -------------------------------------------------------------------------- */
/* This function updates the simple quoted status if needed.                  */
/* If the char is a simple quote and the char is outside a double quote then  */
/* if no lonely single quote exists then a new quoted part begin, otherwise   */
/* the quoted part arrived to an end                                          */
/* Inputs :                                                                   */
/*  - int i : the index to check                                              */
/*  - int *data : array that contained infos about the current situation      */
/*  - const char *str : the command line to check                             */
/* Return :                                                                   */
/*  - None                                                                    */
/* ************************************************************************** */
void	check_simple_quotes(int i, int *data, const char *str)
{
	if (str[i] == '\'' && data[EXT_OPEN_D_QUOTE] == 0)
	{
		if (data[EXT_OPEN_S_QUOTE] == 0)
			data[EXT_OPEN_S_QUOTE] = 1;
		else if (data[EXT_OPEN_S_QUOTE] == 1)
			data[EXT_OPEN_S_QUOTE] = 0;
	}
}

/* ************************************************************************** */
/*                             check_parenthesis                              */
/* -------------------------------------------------------------------------- */
/* This function updates the parenthesis status if needed.                    */
/* If the char is a bracket ('(' or ')') and the char isn't quoted then the   */
/* number of open or close parenthesis is update (incremented)                */
/* Inputs :                                                                   */
/*  - int i : the index to check                                              */
/*  - int *data : array that contained infos about the current situation      */
/*  - const char *str : the command line to check                             */
/* Return :                                                                   */
/*  - None                                                                    */
/* ************************************************************************** */
void	check_parenthesis(int i, int *data, const char *str)
{
	if (str[i] == '(' && data[EXT_OPEN_S_QUOTE] == 0
		&& data[EXT_OPEN_D_QUOTE] == 0)
	{
		data[EXT_OPENING_PARENTHESIS]++;
	}
	if (str[i] == ')' && data[EXT_OPEN_S_QUOTE] == 0
		&& data[EXT_OPEN_D_QUOTE] == 0)
	{
		data[EXT_CLOSING_PARENTHESIS]++;
	}
}

/* ************************************************************************** */
/*                              has_wrong_format                              */
/* -------------------------------------------------------------------------- */
/* This function checks if the string is well formatted (every open quotes    */
/* has been closed, their is as much opening parenthesis than closing ones)   */
/* Inputs :                                                                   */
/*  - int *data : array that contained infos about the current situation      */
/*  - const char *str : the command line to check                             */
/* Return :                                                                   */
/*  - 0 : if the string is well formatted                                     */
/*  - 1 : otherwise                                                           */
/* ************************************************************************** */
int	has_wrong_format(int *data, const char *str)
{
	if (data[EXT_OPEN_S_QUOTE] == 1)
		return (display_error("Wrong number of external '\n"), 1);
	if (data[EXT_OPEN_D_QUOTE] == 1)
		return (display_error("Wrong number of external \"\n"), 1);
	if (data[EXT_OPENING_PARENTHESIS] != data[EXT_CLOSING_PARENTHESIS])
		return (display_error("Wrong number of parentheses\n"), 1);
	if (str[data[START]] == ')' || str[data[END]] == '(')
		return (display_error("Wrong format of parentheses\n"), 1);
	return (0);
}

/* ************************************************************************** */
/*                              has_wrong_start                               */
/* -------------------------------------------------------------------------- */
/* This function checks if c is a command separator. If it is then that mean  */
/* that the syntax is incorrect and an error message is display.              */
/* Inputs :                                                                   */
/*  - const char c : the char to add to the error message display             */
/*  - const char c2 : the char to use if c == '\0'                            */
/* Return :                                                                   */
/*  - 0 : if the string is well formatted                                     */
/*  - 1 : otherwise                                                           */
/* ************************************************************************** */
int	has_wrong_start(const char c, const char c2)
{
	if (is_a_command_separator(c))
	{
		if (c != '\0')
			display_syntax_error(c);
		else
			display_syntax_error(c2);
		return (1);
	}
	return (0);
}
