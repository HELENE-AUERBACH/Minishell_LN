/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_dll_element.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 13:04:21 by hauerbac          #+#    #+#             */
/*   Updated: 2024/03/01 13:06:57 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_dll_el	*new_dll_element(void *content)
{
	t_dll_el	*el;

	el = (t_dll_el *) malloc(sizeof(t_dll_el));
	if (!el)
		return (NULL);
	el->content = content;
	el->prev = NULL;
	el->next = NULL;
	return (el);
}
