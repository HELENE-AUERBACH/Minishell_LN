/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/07 16:41:23 by hauerbac          #+#    #+#             */
/*   Updated: 2023/05/07 16:41:31 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	result;
	size_t	i;
	size_t	j;

	result = 0;
	i = 0;
	j = 0;
	while ((s1[i] != '\0' || s2[j] != '\0') && (i < n || j < n))
	{
		result = (unsigned char) s1[i] - (unsigned char) s2[j];
		if (result != 0)
			return (result);
		if (s1[i] != '\0')
			i++;
		if (s2[j] != '\0')
			j++;
	}
	return (result);
}
