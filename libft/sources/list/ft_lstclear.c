/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 18:04:36 by hauerbac          #+#    #+#             */
/*   Updated: 2023/05/11 08:55:47 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*previous;

	if (lst)
	{
		previous = *lst;
		while (*lst)
		{
			*lst = (*lst)->next;
			ft_lstdelone(previous, del);
			previous = *lst;
		}
		lst = NULL;
	}
}
