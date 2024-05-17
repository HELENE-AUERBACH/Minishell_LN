/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 11:24:44 by hauerbac          #+#    #+#             */
/*   Updated: 2023/05/10 10:43:23 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

static size_t	ft_get_substr_len(size_t s_len, unsigned int start, size_t len)
{
	if (s_len - start < len)
		return (s_len - start);
	else
		return (len);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*ptr;
	size_t	size;

	size = ft_strlen(s);
	if (size <= start)
	{
		ptr = (char *) malloc(sizeof(char));
		if (ptr)
		{
			ptr[0] = '\0';
			return (ptr);
		}
		return (NULL);
	}
	else
	{
		size = ft_get_substr_len(size, start, len);
		ptr = (char *) malloc((size + 1) * sizeof(char));
		if (ptr)
		{
			ft_strlcpy(ptr, s + start, size + 1);
			return (ptr);
		}
	}
	return (NULL);
}
