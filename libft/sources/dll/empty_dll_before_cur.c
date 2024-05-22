/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   empty_dll_before_cur.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 12:46:42 by hauerbac          #+#    #+#             */
/*   Updated: 2024/05/21 13:24:45 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	empty_dll_before_cur(t_dll *lst, t_list *current,
		void (*del)(void *))
{
	t_dll_el	*el_ptr;

	if (lst && lst->size > 0 && del)
	{
		el_ptr = lst->head;
		while (el_ptr && (!current || (current && current->content \
				&& el_ptr->content != current->content)))
		{
			el_ptr = el_ptr->next;
			del_el_at_dll_head(lst, del);
		}
		if (lst->size == 0)
		{
			if (lst->head)
				lst->head = NULL;
			if (lst->tail)
				lst->tail = NULL;
		}
	}
}
