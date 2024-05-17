/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dll_clear.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 15:15:25 by hauerbac          #+#    #+#             */
/*   Updated: 2024/03/01 15:29:28 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	safe_free_dll(t_dll **lst)
{
	if (lst && *lst)
	{
		if ((*lst)->head)
			(*lst)->head = NULL;
		if ((*lst)->tail)
			(*lst)->tail = NULL;
		free(*lst);
		*lst = NULL;
	}
}

void	dll_clear(t_dll **lst, void (*del)(void *))
{
	t_dll_el	*el_ptr;

	if (lst && *lst)
	{
		el_ptr = (*lst)->head;
		while (el_ptr)
		{
			el_ptr = el_ptr->next;
			del_el_at_dll_head(*lst, del);
		}
		safe_free_dll(lst);
	}
}
