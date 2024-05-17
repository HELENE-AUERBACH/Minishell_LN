/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   insert_at_dll_head.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 13:12:11 by hauerbac          #+#    #+#             */
/*   Updated: 2024/03/01 14:16:35 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	insert_at_dll_head(t_dll *lst, void *content)
{
	t_dll_el	*new_el;

	if (!lst)
		return (-3);
	new_el = new_dll_element(content);
	if (!new_el)
		return (-1);
	if (lst->size > 0)
	{
		if (lst->head)
			lst->head->prev = new_el;
		else
		{
			free(new_el);
			new_el = NULL;
			return (-2);
		}
	}
	if (lst->size == 0)
		lst->tail = new_el;
	new_el->next = lst->head;
	lst->head = new_el;
	lst->size++;
	return (0);
}
