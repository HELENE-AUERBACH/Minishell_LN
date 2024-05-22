/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   empty_dll_after_cur.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 14:27:42 by hauerbac          #+#    #+#             */
/*   Updated: 2024/05/21 17:20:19 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	delete_el_ptr(t_dll *lst, t_dll_el *ptr, void (*del)(void *))
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
		lst->size--;
	}
}

void	empty_dll_after_cur(t_dll *lst, t_list *current,
		void (*del)(void *))
{
	t_dll_el	*el_ptr;
	t_dll_el	*mem_el_ptr;
	int			found;

	if (lst && lst->size > 0 && del)
	{
		el_ptr = lst->head;
		found = 0;
		while (el_ptr && !found && current && current->content)
		{
			if (el_ptr->content == current->content)
				found = 1;
			el_ptr = el_ptr->next;
		}
		while (el_ptr)
		{
			mem_el_ptr = el_ptr;
			delete_el_ptr(lst, el_ptr, del);
			el_ptr = mem_el_ptr->next;
		}
		if (lst->size == 0 && lst->head)
			lst->head = NULL;
		if (lst->size == 0 && lst->tail)
			lst->tail = NULL;
	}
}
