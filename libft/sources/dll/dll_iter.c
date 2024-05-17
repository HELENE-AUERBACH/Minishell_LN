/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dll_iter.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 15:30:20 by hauerbac          #+#    #+#             */
/*   Updated: 2024/03/11 18:51:27 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	dll_iter(t_dll *lst, int (*f)(t_dll_el *))
{
	t_dll_el	*element_ptr;
	int			result;

	if (lst && lst->size > 0)
	{
		element_ptr = lst->head;
		while (element_ptr)
		{
			result = f(element_ptr);
			if (result != 0)
				return (result);
			element_ptr = element_ptr->next;
		}
		return (0);
	}
	return (-2);
}
