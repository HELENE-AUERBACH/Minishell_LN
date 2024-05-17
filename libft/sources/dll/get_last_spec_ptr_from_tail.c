/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_last_spec_ptr_from_tail.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 16:19:17 by hauerbac          #+#    #+#             */
/*   Updated: 2024/03/18 16:19:56 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_dll_el	*get_last_spec_ptr_from_tail(t_dll *lst, int (*f)(void *))
{
	t_dll_el	*el_ptr;

	if (lst && lst->size > 0 && lst->tail)
	{
		el_ptr = lst->tail;
		while (el_ptr)
		{
			if (f(el_ptr->content) == 1)
				return (el_ptr);
			el_ptr = el_ptr->prev;
		}
	}
	return (NULL);
}
