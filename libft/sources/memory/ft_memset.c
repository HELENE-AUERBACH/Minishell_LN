/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 13:51:05 by hauerbac          #+#    #+#             */
/*   Updated: 2023/05/04 16:43:23 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	char_from_asciicode;
	char			*tab;

	tab = (char *) s;
	char_from_asciicode = (unsigned char) c;
	i = 0;
	while (i < n)
	{
		tab[i] = char_from_asciicode;
		i++;
	}
	return (s);
}
