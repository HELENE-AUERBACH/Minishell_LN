/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ctypes_split_lexer_minishell.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmorice <rmorice@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 12:25:19 by hauerbac          #+#    #+#             */
/*   Updated: 2024/09/13 14:53:00 by rmorice          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_minishell.h"

/* ************************************************************************** */
/*                                 is_a_space                                 */
/* -------------------------------------------------------------------------- */
/* This function checks if the char received is a space (space, horizontal    */
/* tab, vertical tab, form feed, carriage ret)                                */
/* Input :                                                                    */
/*  - const char c : the char to check                                        */
/* Return :                                                                   */
/*  - 1 : if the char received is a space                                     */
/*  - 0 : otherwise                                                           */
/* ************************************************************************** */
int	is_a_space(const char c)
{
	if (c != '\0'
		&& (c == ' ' || c == '\t' || c == '\v' || c == '\f'
			|| c == '\r'))
		return (1);
	return (0);
}

/* ************************************************************************** */
/*                             is_a_metacharacter                             */
/* -------------------------------------------------------------------------- */
/* This function checks if the char received is a metacharacter (null, space, */
/* new line, pipe, ampersand, semicolon, brackets, less, great, horizontal    */
/* tab, vertical tab, form feed, carriage ret)                                */
/* Input :                                                                    */
/*  - const char c : the char to check                                        */
/* Return :                                                                   */
/*  - 1 : if the char received is a metacharacter                             */
/*  - 0 : otherwise                                                           */
/* ************************************************************************** */
int	is_a_metacharacter(const char c)
{
	if (c == '\0' || c == ' ' || c == '\n' || c == '|' || c == '&'
		|| c == ';' || c == '(' || c == ')' || c == '<' || c == '>'
		|| c == '\t' || c == '\v' || c == '\f' || c == '\r')
		return (1);
	return (0);
}

/* ************************************************************************** */
/*                           is_a_command_separator                           */
/* -------------------------------------------------------------------------- */
/* This function checks if the char received is a command separator (null,    */
/* new line, pipe, ampersand, semicolon)                                      */
/* Input :                                                                    */
/*  - const char c : the char to check                                        */
/* Return :                                                                   */
/*  - 1 : if the char received is a command separator                         */
/*  - 0 : otherwise                                                           */
/* ************************************************************************** */
int	is_a_command_separator(const char c)
{
	if (c == '\0' || c == '\n' || c == '|' || c == '&' || c == ';')
		return (1);
	return (0);
}

/* ************************************************************************** */
/*                              is_a_redirection                              */
/* -------------------------------------------------------------------------- */
/* This function checks if we have a redirection (less, great)                */
/* Inputs :                                                                   */
/*  - int i : the index of the char to check                                  */
/*  - int *data : array that contained infos about the current situation      */
/*  - const char *str : the command line that contained the char to check     */
/* Return :                                                                   */
/*  - 1 : if the char received is a redirection (out of quotes)               */
/*  - 0 : otherwise                                                           */
/* ************************************************************************** */
// why do we check if in quotes for redirection and not for separator ???
int	is_a_redirection(int i, int *data, const char *str)
{
	if (data[EXT_OPEN_S_QUOTE] == 0
		&& data[EXT_OPEN_D_QUOTE] == 0
		&& (str[i] == '<' || str[i] == '>'))
		return (1);
	return (0);
}

/* ************************************************************************** */
/*                          is_a_char_of_raw_command                          */
/* -------------------------------------------------------------------------- */
/* This function checks if the char can be interpreted as part of raw command */
/* (If it isn't a redirection nor a command separarator, or if the character  */
/* is quoted or if it is a space and the next character existed and isn't a   */
/* command separator nor a space)                                             */
/* Inputs :                                                                   */
/*  - int i : the index of the char to check                                  */
/*  - int *data : array that contained infos about the current situation      */
/*  - const char *str : the command line that contained the char to check     */
/* Return :                                                                   */
/*  - 1 : if the char received is part of a raw command                       */
/*  - 0 : otherwise                                                           */
/* ************************************************************************** */
int	is_a_char_of_raw_command(int i, int *data, const char *str)
{
	if (!is_a_metacharacter(str[i])
		|| str[i] == '(' || str[i] == ')'
		|| data[EXT_OPEN_S_QUOTE] == 1
		|| data[EXT_OPEN_D_QUOTE] == 1
		|| (i + 1 < data[STR_LEN] && is_a_space(str[i])
			&& !is_a_command_separator(str[i + 1])
			&& !is_a_space(str[i + 1])))
		return (1);
	return (0);
}
