/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 17:42:25 by hauerbac          #+#    #+#             */
/*   Updated: 2023/05/08 17:54:00 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

char	*ft_strdup(const char *s)
{
	size_t	s_len;
	char	*ptr;

	s_len = ft_strlen(s);
	ptr = (char *) malloc((s_len + 1) * sizeof(char));
	if (ptr)
	{
		ft_strlcpy(ptr, s, s_len + 1);
		return (ptr);
	}
	return (NULL);
}
