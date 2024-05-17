/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   del_el_at_dll_tail.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 15:04:29 by hauerbac          #+#    #+#             */
/*   Updated: 2024/03/01 15:04:50 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	del_el_at_dll_tail(t_dll *lst, void (*del)(void *))
{
	t_dll_el	*ptr;

	ptr = lst->tail;
	delete_dll_el(lst, ptr, del);
}
