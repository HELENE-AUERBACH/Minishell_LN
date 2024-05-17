/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_dll.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 19:36:47 by hauerbac          #+#    #+#             */
/*   Updated: 2024/05/15 17:03:30 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	print_el_content(int fd, void *el_content,
			void (*f)(int, void *))
{
	f(fd, el_content);
}

static void	print_dll_el(t_dll_el *el_ptr, int fd, void (*f)(int, void *))
{
	if (el_ptr)
	{
		print_el_content(fd, el_ptr->content, f);
		ft_putchar_fd('\n', fd);
	}
}

void	print_dll(t_dll *lst, int fd, void (*f)(int, void *))
{
	t_dll_el	*element_ptr;

	if (lst)
	{
		element_ptr = lst->head;
		while (element_ptr)
		{
			print_dll_el(element_ptr, fd, f);
			element_ptr = element_ptr->next;
		}
		ft_putstr_fd("Doubly linked list size : ", fd);
		ft_putnbr_fd((int) lst->size, fd);
		ft_putchar_fd('\n', fd);
	}
}
