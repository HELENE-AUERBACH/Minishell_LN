/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_3_minishell.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 12:35:33 by hauerbac          #+#    #+#             */
/*   Updated: 2024/05/30 12:37:35 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

unsigned int	ft_spaces_truncating(const char *str)
{
	unsigned int	i;

	i = 0;
	while (str[i]
		&& (str[i] == ' ' || str[i] == '\f'
			|| str[i] == '\n' || str[i] == '\r'
			|| str[i] == '\t' || str[i] == '\v'))
		i++;
	return (i);
}

unsigned int	ft_chartodigit(const char c)
{
	unsigned int	digit;

	digit = 0;
	if (c >= '0' && c <= '9')
		digit = c - '0';
	if (c >= 'A' && c <= 'Z')
		digit = c - 'A' + 10;
	if (c >= 'a' && c <= 'z')
		digit = c - 'a' + 10;
	return (digit);
}
