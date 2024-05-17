/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   empty_dll.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 14:14:53 by hauerbac          #+#    #+#             */
/*   Updated: 2024/03/04 14:19:05 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	empty_dll(t_dll *lst, void (*del)(void *))
{
	t_dll_el	*el_ptr;

	if (lst)
	{
		el_ptr = lst->head;
		while (el_ptr)
		{
			el_ptr = el_ptr->next;
			del_el_at_dll_head(lst, del);
		}
		if (lst->head)
			lst->head = NULL;
		if (lst->tail)
			lst->tail = NULL;
	}
}
