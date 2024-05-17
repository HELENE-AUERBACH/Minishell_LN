/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 17:09:02 by hauerbac          #+#    #+#             */
/*   Updated: 2023/05/08 17:26:29 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static unsigned int	ft_spaces_truncating(const char *str)
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

static int	ft_error_or_sign(unsigned int boundaries[2], const char *str)
{
	int				result;
	unsigned int	i;

	result = 0;
	boundaries[0] = -1;
	boundaries[1] = -2;
	result = 1;
	i = ft_spaces_truncating(str);
	if (str[i] && (str[i] == '+' || str[i] == '-'))
	{
		if (str[i] == '-')
			result = -1 * result;
		i++;
	}
	if (str[i] && str[i] >= 48 && str[i] <= 57)
		boundaries[0] = (unsigned int) i;
	while (str[i] && str[i] >= 48 && str[i] <= 57)
		boundaries[1] = (unsigned int) i++;
	if (boundaries[0] > boundaries[1])
		result = 0;
	return (result);
}

int	ft_atoi(const char *nptr)
{
	unsigned int	boundaries[2];
	unsigned int	i;
	int				error_or_sign;
	int				result;

	error_or_sign = ft_error_or_sign(boundaries, nptr);
	result = 0;
	if (error_or_sign == 0)
		return (0);
	else
	{
		i = boundaries[0];
		result = nptr[i++] - 48;
		while (i <= boundaries[1])
			result = (result * 10) + (nptr[i++] - 48);
		result *= error_or_sign;
	}
	return (result);
}
