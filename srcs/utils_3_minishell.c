/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_3_minishell.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmorice <rmorice@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 12:35:33 by hauerbac          #+#    #+#             */
/*   Updated: 2024/09/16 14:40:27 by rmorice          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                            ft_spaces_truncating                            */
/* -------------------------------------------------------------------------- */
/* This function returns the first index of a string that isn't a space (' ', */
/* form freed, newline, carriage ret, horizontal tab, vertical tab)           */
/* Input :                                                                    */
/*  - const char *str : the string into which we are looking                  */
/* Return :                                                                   */
/*  - int : the index of the first character of str that isn't a space        */
/* ************************************************************************** */
// To check : is it possible to transform the function that have the same name in atoi.c
// file into a static function ???
unsigned int	ft_spaces_truncating(const char *str)
{
	unsigned int	i;

	i = 0;
	while (str[i]
		&& (str[i] == ' ' || str[i] == '\f'
			|| str[i] == '\n' || str[i] == '\r'
			|| str[i] == '\t' || str[i] == '\v'))
		i++;
	return (i);
}

/* ************************************************************************** */
/*                               ft_chartodigit                               */
/* -------------------------------------------------------------------------- */
/* This function converts a char into the numerical value associated          */
/* (numerical value == themselves, alphabatic value == alphabetic index + 10) */
/* Input :                                                                    */
/*  - const char c : the char to convert into a numerical value               */
/* Return :                                                                   */
/*  - unsigned int : the numerical value obtained after conversion            */
/* ************************************************************************** */
unsigned int	ft_chartodigit(const char c)
{
	unsigned int	digit;

	digit = 0;
	if (c >= '0' && c <= '9')
		digit = c - '0';
	if (c >= 'A' && c <= 'Z')
		digit = c - 'A' + 10;
	if (c >= 'a' && c <= 'z')
		digit = c - 'a' + 10;
	return (digit);
}
