/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dll_map.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 15:39:44 by hauerbac          #+#    #+#             */
/*   Updated: 2024/03/01 18:12:57 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_dll	*dll_map(t_dll *lst, void *(*f)(void *), void (*del)(void *))
{
	t_dll		*new_dll;
	t_dll_el	*el;

	new_dll = new_empty_dll();
	if (lst && lst->size > 0 && new_dll)
	{
		el = lst->head;
		while (el)
		{
			if (!insert_at_dll_tail(new_dll, f(el->content)))
			{
				if (new_dll)
					dll_clear(&new_dll, del);
				return (NULL);
			}
			el = el->next;
		}
	}
	return (new_dll);
}
