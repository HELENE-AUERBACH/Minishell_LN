/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_empty_dll.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 12:56:34 by hauerbac          #+#    #+#             */
/*   Updated: 2024/03/01 13:03:08 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_dll	*new_empty_dll(void)
{
	t_dll	*lst;

	lst = (t_dll *) malloc(sizeof(t_dll));
	if (!lst)
		return (NULL);
	lst->head = NULL;
	lst->tail = NULL;
	lst->size = 0;
	return (lst);
}
