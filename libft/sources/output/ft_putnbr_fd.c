/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 09:41:10 by hauerbac          #+#    #+#             */
/*   Updated: 2023/05/10 09:44:07 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	long	n_into_long;
	int		digit_to_char;

	n_into_long = n;
	if (n_into_long < 0)
	{
		write(fd, "-", 1);
		n_into_long = n_into_long * -1;
	}
	if (n_into_long < 10)
	{
		digit_to_char = n_into_long + '0';
		write(fd, &digit_to_char, 1);
	}
	else
	{
		ft_putnbr_fd(n_into_long / 10, fd);
		ft_putnbr_fd(n_into_long % 10, fd);
	}
}
