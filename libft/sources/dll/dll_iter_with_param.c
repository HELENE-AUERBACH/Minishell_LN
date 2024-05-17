/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dll_iter_with_param.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 15:45:18 by hauerbac          #+#    #+#             */
/*   Updated: 2024/03/12 15:49:27 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	dll_iter_with_param(t_dll *lst, void *param, int (*f)(t_dll_el *,
		void *))
{
	t_dll_el	*element_ptr;
	int			result;

	if (lst && lst->size > 0)
	{
		element_ptr = lst->head;
		while (element_ptr)
		{
			result = f(element_ptr, param);
			if (result != 0)
				return (result);
			element_ptr = element_ptr->next;
		}
		return (0);
	}
	return (-2);
}
