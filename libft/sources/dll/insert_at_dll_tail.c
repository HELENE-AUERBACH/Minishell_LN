/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   insert_at_dll_tail.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 13:29:04 by hauerbac          #+#    #+#             */
/*   Updated: 2024/03/01 14:17:19 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	insert_at_dll_tail(t_dll *lst, void *content)
{
	t_dll_el	*new_el;

	if (!lst)
		return (-3);
	new_el = new_dll_element(content);
	if (!new_el)
		return (-1);
	if (lst->size > 0)
	{
		if (lst->tail)
			lst->tail->next = new_el;
		else
		{
			free(new_el);
			new_el = NULL;
			return (-2);
		}
	}
	if (lst->size == 0)
		lst->head = new_el;
	new_el->prev = lst->tail;
	lst->tail = new_el;
	lst->size++;
	return (0);
}
