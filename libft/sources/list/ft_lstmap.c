/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 18:59:49 by hauerbac          #+#    #+#             */
/*   Updated: 2023/05/10 18:59:52 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_list;
	t_list	*previous;
	t_list	*first;

	first = NULL;
	previous = NULL;
	while (lst)
	{
		new_list = (t_list *) malloc(sizeof(t_list));
		if (!new_list)
		{
			if (first)
				ft_lstclear(&first, del);
			return (NULL);
		}
		new_list->content = f(lst->content);
		new_list->next = NULL;
		if (previous)
			previous->next = new_list;
		else
			first = new_list;
		previous = new_list;
		lst = lst->next;
	}
	return (first);
}
