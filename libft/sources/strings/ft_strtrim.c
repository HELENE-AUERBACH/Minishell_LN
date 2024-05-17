/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 14:11:11 by hauerbac          #+#    #+#             */
/*   Updated: 2023/05/19 09:39:08 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

static void	ft_set_left_boundary(size_t boundaries[2], const char *s1,
		const char *set, size_t set_len)
{
	size_t	i;
	size_t	j;

	i = boundaries[0];
	j = 0;
	while (j < set_len)
	{
		while (i <= boundaries[1] && s1[i] == set[j])
			i++;
		if (boundaries[0] < i)
		{
			boundaries[0] = i;
			j = -1;
		}
		j++;
	}
}

static void	ft_set_right_boundary(size_t boundaries[2], const char *s1,
		const char *set, size_t set_len)
{
	size_t	i;
	size_t	j;

	i = boundaries[1];
	j = 0;
	while (j < set_len)
	{
		while (i >= boundaries[0] && s1[i] == set[j])
			i--;
		if (boundaries[1] > i)
		{
			boundaries[1] = i;
			j = -1;
		}
		j++;
	}
}

static size_t	ft_get_boundaries(size_t boundaries[2], const char *s1,
		const char *set)
{
	size_t	s1_len;
	size_t	set_len;

	s1_len = ft_strlen(s1);
	boundaries[0] = 0;
	boundaries[1] = s1_len - 1;
	set_len = ft_strlen(set);
	if (set_len == 0)
		return (s1_len);
	ft_set_left_boundary(boundaries, s1, set, set_len);
	if (boundaries[0] > boundaries[1])
		return (0);
	ft_set_right_boundary(boundaries, s1, set, set_len);
	return (boundaries[1] - boundaries[0] + 1);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*result;
	size_t	trimmed_len;
	size_t	boundaries[2];

	trimmed_len = ft_get_boundaries(boundaries, s1, set);
	result = (char *) malloc((trimmed_len + 1) * sizeof(char));
	if (!result)
		return (NULL);
	if (trimmed_len == 0)
		result[0] = '\0';
	else
		ft_strlcpy(result, s1 + boundaries[0], trimmed_len + 1);
	return (result);
}
