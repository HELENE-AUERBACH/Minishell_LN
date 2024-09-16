/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2_minishell.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmorice <rmorice@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 12:28:48 by hauerbac          #+#    #+#             */
/*   Updated: 2024/09/12 14:26:38 by rmorice          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_minishell.h"

/* ************************************************************************** */
/*                                  ft_base                                   */
/* -------------------------------------------------------------------------- */
/* This function determines in which base a number contained in str is given  */
/* Inputs :                                                                   */
/*  - unsigned int *i : the first character to look at                        */
/*  - const char *str : the string that contain the number that we study      */
/* Return :                                                                   */
/*  - unsigned int : the numerical alue associated at the base (2, 8, 10, 16) */
/* ************************************************************************** */
static unsigned int	ft_base(unsigned int *i, const char *str)
{
	unsigned int	base;

	base = 0;
	if (str[*i] && str[*i] == '0')
	{
		while (str[*i + 1] && str[*i + 1] == '0')
			*i += 1;
		if (str[*i + 1] == '\0')
			base = 10;
		else
			*i += 1;
		if (str[*i] && str[*i] == 'b')
			base = 2;
		else if (str[*i] && (str[*i] == 'x' || str[*i] == 'X'))
			base = 16;
		else
			base = 8;
		if (str[*i] && (str[*i] == 'b' || str[*i] == 'x' \
				|| str[*i] == 'X'))
			*i += 1;
	}
	else if (str[*i])
		base = 10;
	return (base);
}

/* ************************************************************************** */
/*                        ft_is_a_valid_digit_in_base                         */
/* -------------------------------------------------------------------------- */
/* This function checks if the char received is valid in the base specified   */
/* Inputs :                                                                   */
/*  - unsigned int base : the numerical value associated to the base study    */
/*  - const char c : the char that we check                                   */
/* Return :                                                                   */
/*  - 1 : if c is valid in base                                               */
/*  - 0 : otherwise                                                           */
/* ************************************************************************** */
// base <= 36 ?????????
static unsigned int	ft_is_a_valid_digit_in_base(unsigned int base, \
			const char c)
{
	unsigned int	boolean;

	boolean = 0;
	if (base <= 36 && c >= 48 && ((base == 2 && c <= 49) \
		|| (base == 8 && c <= 55) || (base == 10 && c <= 57) \
		|| (base == 16 && (c <= 57 || (c >= 65 && c <= 70) \
		|| (c >= 97 && c <= 102)))))
		boolean = 1;
	return (boolean);
}

/* ************************************************************************** */
/*                         ft_error_or_sign_and_base                          */
/* -------------------------------------------------------------------------- */
/* This function determines if a number is contained in str or if an error    */
/* occured. If there is a number, this function determines its sign, base and */
/* index boundaries in str                                                    */
/* if there is more than one sign, if at least one value contained in str     */
/* isn't part of those allow in base, then an error is detected               */
/* To do so, we remove spaces at the beginning of str, then checks if the     */
/* first char that isn't a space is a sign. After that the base in which the  */
/* number contained in str is express is determined and the boundaries        */
/* indexes of the number part in str are determined                           */
/* Inputs :                                                                   */
/*  - unsigned int boundaries[2]  */
/*  - unsigned int *base : numerical value of the base in which nb is express */
/*  - unsigned int *i : the index in str (update to the last char study)      */
/*  - const char *str : the string that we checked                            */
/* Return :                                                                   */
/*  - -1 || 1 : the sign multiplier, if everything goes well                  */
/*  - 0 : if an error occured                                                 */
/* ************************************************************************** */
// base > 36 ????
// if !str or !str[*i] -> we try to access it => SegFault, can that append ???
// even if we pretend that SegFault doesn't occured, if str==<spaces>"\0", result == 1
// hmm... boundaries[] is of type UNSIGNED INT yet you initialised its values with
// -1 and -2 ^^' how ???
static int	ft_error_or_sign_and_base(unsigned int boundaries[2],
			unsigned int *base, unsigned int *i, const char *str)
{
	int	result;

	boundaries[0] = -1;
	boundaries[1] = -2;
	*base = 0;
	result = 1;
	*i = ft_spaces_truncating(str);
	if (str[*i] && (str[*i] == '+' || str[*i] == '-') && str[(*i) + 1])
	{
		if (str[*i] == '-')
			result = -1 * result;
		*i += 1;
	}
	*base = ft_base(i, str);
	if (*base > 36 || *base == 1 || *base == 0)
		result = 0;
	if (ft_is_a_valid_digit_in_base(*base, str[*i]) == 1)
		boundaries[0] = (unsigned int) *i;
	while (str[*i] && ft_is_a_valid_digit_in_base(*base, str[*i]) == 1)
		boundaries[1] = (unsigned int)((*i)++);
	if (str[*i] || *base <= 0 || boundaries[0] > boundaries[1])
		result = 0;
	return (result);
}

/* ************************************************************************** */
/*                                 ft_strtoll                                 */
/* -------------------------------------------------------------------------- */
/* This function converts a string into an unsigned (???) long long while     */
/* expressing it in base 10 if needed                                         */
/* Inputs :                                                                   */
/*  - const char *nptr  */
/*  - char **endptr   */
/*  - int *sign : the sign multiplier of the number obtained                  */
/* Return :                                                                   */
/*  - unsigned long long : the numerical value converts from its base         */
/* ************************************************************************** */
// euh.... LLONG_MIN is negative yet we return it when sign == -1 && result - 1 > LLONG MAX
// but the return value is supposed to be UNSIGNED long long ^^'
unsigned long long	ft_strtoll(const char *nptr, char **endptr, int *sign)
{
	unsigned int		boundaries[2];
	unsigned int		base;
	unsigned int		i;
	unsigned long long	result;

	*sign = ft_error_or_sign_and_base(boundaries, &base, &i, nptr);
	result = 0;
	*endptr = NULL;
	if (*sign == 0)
	{
		if (endptr)
			*endptr = (char *) nptr + i;
		return (0);
	}
	i = boundaries[0];
	while (i <= boundaries[1])
	{
		result = (result * base) + ft_chartodigit(nptr[i]);
		if (*sign == 1 && result > LLONG_MAX)
			return (LLONG_MAX);
		if (*sign == -1 && result - 1 > LLONG_MAX)
			return (LLONG_MIN);
		*endptr = (char *) nptr + ++i;
	}
	return (result);
}

/* ************************************************************************** */
/*                        display_err_with_2_prefixes                         */
/* -------------------------------------------------------------------------- */
/* This function displays an error message in the error output. if no message */
/* is given in input of the function it will display a default error message  */
/* Otherwise the error message will be formatted as follow                    */
/* <prefix1>:<prefix2>:<error_msg>                                            */
/* (if prefix1 or prefix2 does exist it is skip)                              */
/* Inputs :                                                                   */
/*  - char *prefix1 : the first prefix to display before the error message    */
/*  - char *prefix2 : the second prefix to display before the error message   */
/*  - char *error_msg : the error message to display. If NULL -> default msg  */
/* Return :                                                                   */
/*  - None                                                                    */
/* ************************************************************************** */
// ???
// if !err_msg && !prefix1 && !prefix2 => "Error\n::"
// if !err_msg && prefix1 && !prefix2 => "Error\n"<prefix1>"::"
// if !err_msg && !prefix1 && prefix2 => "Error\n:"<prefix2>":"
// if !err_msg && prefix1 && prefix2 => "Error\n"<prefix1>":"<prefix2>":"
// if err_msg && !prefix1 && !prefix2 => "::"<err_msg>
// if err_msg && prefix1 && !prefix2 => <prefix1>"::"<err_msg>
// if err_msg && !prefix1 && prefix2 => ":"<prefix2>":"<err_msg>
// is it really what we wanted ???
void	display_err_with_2_prefixes(const char *prefix1, const char *prefix2,
		const char *error_msg)
{
	if (!error_msg)
		write(2, "Error\n", 6);
	if (prefix1)
		write(2, prefix1, ft_strlen(prefix1));
	write(2, ":", 1);
	if (prefix2)
		write(2, prefix2, ft_strlen(prefix2));
	write(2, ":", 1);
	if (error_msg)
		write(2, error_msg, ft_strlen(error_msg));
}
