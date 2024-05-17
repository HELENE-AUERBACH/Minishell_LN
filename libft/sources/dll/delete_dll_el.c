/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete_dll_el.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 14:38:08 by hauerbac          #+#    #+#             */
/*   Updated: 2024/03/01 17:55:12 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	safe_free_dll_element_ptr(t_dll_el **ptr)
{
	if (ptr && *ptr)
	{
		free(*ptr);
		*ptr = NULL;
	}
}

void	delete_dll_el(t_dll *lst, t_dll_el *ptr, void (*del)(void *))
{
	if (lst && lst->size > 0 && ptr && del)
	{
		if (lst->head && lst->head == ptr)
			lst->head = ptr->next;
		if (lst->tail && lst->tail == ptr)
			lst->tail = ptr->prev;
		if (lst->size > 1)
		{
			if (ptr->next)
				ptr->next->prev = ptr->prev;
			if (ptr->prev)
				ptr->prev->next = ptr->next;
			ptr->next = NULL;
			ptr->prev = NULL;
		}
		del(ptr->content);
		safe_free_dll_element_ptr(&ptr);
		lst->size--;
	}
}
