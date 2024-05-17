/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   del_el_at_dll_head.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 14:58:53 by hauerbac          #+#    #+#             */
/*   Updated: 2024/03/01 15:03:32 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	del_el_at_dll_head(t_dll *lst, void (*del)(void *))
{
	t_dll_el	*ptr;

	ptr = lst->head;
	delete_dll_el(lst, ptr, del);
}
