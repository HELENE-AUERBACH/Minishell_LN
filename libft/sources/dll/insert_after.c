/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   insert_after.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 14:18:18 by hauerbac          #+#    #+#             */
/*   Updated: 2024/03/01 18:15:59 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	insert_after(t_dll *lst, t_dll_el *ptr, void *content)
{
	t_dll_el	*new_el;

	if (!lst)
		return (-3);
	if (lst->size <= 1 || ptr == lst->tail)
	{
		if (lst->tail && ptr != lst->tail)
			return (-2);
		return (insert_at_dll_tail(lst, content));
	}
	new_el = new_dll_element(content);
	if (!new_el)
		return (-1);
	ptr->next->prev = new_el;
	new_el->next = ptr->next;
	new_el->prev = ptr;
	ptr->next = new_el;
	lst->size++;
	return (0);
}
