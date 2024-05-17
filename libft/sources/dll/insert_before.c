/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   insert_before.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 13:43:51 by hauerbac          #+#    #+#             */
/*   Updated: 2024/03/01 18:15:10 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	insert_before(t_dll *lst, t_dll_el *ptr, void *content)
{
	t_dll_el	*new_el;

	if (!lst)
		return (-3);
	if (lst->size <= 1 || ptr == lst->head)
	{
		if (lst->head && ptr != lst->head)
			return (-2);
		return (insert_at_dll_head(lst, content));
	}
	new_el = new_dll_element(content);
	if (!new_el)
		return (-1);
	new_el->prev = ptr->prev;
	ptr->prev->next = new_el;
	ptr->prev = new_el;
	new_el->next = ptr;
	lst->size++;
	return (0);
}
