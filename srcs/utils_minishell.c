/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_minishell.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmorice <rmorice@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 16:15:14 by hauerbac          #+#    #+#             */
/*   Updated: 2024/09/25 15:51:47 by rmorice          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_minishell.h"

/* ************************************************************************** */
/*                               display_error                                */
/* -------------------------------------------------------------------------- */
/* This function displays an error message in the error output. if no message */
/* is given in input of the function it will display a default error message  */
/* Input :                                                                    */
/*  - char *error_msg : the error message to display. If NULL -> default msg  */
/* Return :                                                                   */
/*  - None                                                                    */
/* ************************************************************************** */
void	display_error(const char *error_msg)
{
	if (!error_msg)
		write(2, "Error\n", 6);
	if (error_msg)
		write(2, error_msg, ft_strlen(error_msg));
}

/* ************************************************************************** */
/*                          display_err_with_prefix                           */
/* -------------------------------------------------------------------------- */
/* This function displays an error message in the error output. if no message */
/* is given in input of the function it will display a default error message  */
/* Otherwise the error message will be formatted as <prefix>:<error_msg>      */
/* Inputs :                                                                   */
/*  - char *prefix : the prefix to display before the error message           */
/*  - char *error_msg : the error message to display. If NULL -> default msg  */
/* Return :                                                                   */
/*  - None                                                                    */
/* ************************************************************************** */
void	display_err_with_prefix(const char *prefix, const char *error_msg)
{
	if (!error_msg)
		write(2, "Error\n", 6);
	if (prefix)
		write(2, prefix, ft_strlen(prefix));
	write(2, ":", 1);
	if (error_msg)
		write(2, error_msg, ft_strlen(error_msg));
}

/* ************************************************************************** */
/*                            display_syntax_error                            */
/* -------------------------------------------------------------------------- */
/* This function displays an error message in the error output. The character */
/* c given in input is added to the message. If c == '\0' then c2 replaces it */
/* in the error message.                                                      */
/* Input :                                                                    */
/*  - char *error_msg : the error message to display. If NULL -> default msg  */
/* Return :                                                                   */
/*  - None                                                                    */
/* ************************************************************************** */
void	display_syntax_error(const char c)
{
	display_error("Syntax error near unexpected token `");
	write(2, &c, 1);
	write(2, "'\n", 2);
}

/* ************************************************************************** */
/*                            ft_strjoin_with_free                            */
/* -------------------------------------------------------------------------- */
/* This function creates a string that is the combination of s1 and s2. (s2   */
/* is add at the end of s1)                                                   */
/* s1 and s2 are then free and the string created is return                   */
/* Input :                                                                    */
/*  - char *s1 : the first string to join                                     */
/*  - char *s2 : the string to add at the end of the previous string          */
/* Return :                                                                   */
/*  - char *: the string obtained by adding s2 at the end of s1               */
/* ************************************************************************** */
char	*ft_strjoin_with_free(char *s1, char *s2)
{
	char	*result;
	size_t	s1_len;
	size_t	s2_len;

	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	result = (char *) malloc((s1_len + s2_len + 1) * sizeof(char));
	if (!result)
		return (NULL);
	if (s1_len == 0 && s2_len == 0)
		result[0] = '\0';
	else
	{
		if (s1_len > 0)
		{
			ft_strlcpy(result, s1, s1_len + 1);
			free(s1);
		}
		if (s2_len > 0)
		{
			ft_strlcpy(result + s1_len, s2, s2_len + 1);
			free(s2);
		}
	}
	return (result);
}

/* ************************************************************************** */
/*                         ends_with_a_closing_brace                          */
/* -------------------------------------------------------------------------- */
/* This function checks if "t->src" contained a closing brace (at j or a      */
/* greater index)                                                             */
/* Input :                                                                    */
/*  - t_token *t : a structure that contained datas about the current token   */
/*  - int j : the first index to check                                        */
/* Return :                                                                   */
/*  - 1 : if t->src contained a closing brace                                 */
/*  - 0 : otherwise                                                           */
/* ************************************************************************** */
int	ends_with_a_closing_brace(t_token *t, int j)
{
	while (j < t->src_len && t->src[j] && t->src[j] != '}')
		j++;
	if (!(j < t->src_len && t->src[j] && t->src[j] == '}'))
		return (0);
	return (1);
}
