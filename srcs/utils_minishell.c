/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_minishell.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 16:15:14 by hauerbac          #+#    #+#             */
/*   Updated: 2024/06/05 11:53:30 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_minishell.h"

void	display_error(const char *error_msg)
{
	if (!error_msg)
		write(2, "Error\n", 6);
	if (error_msg)
		write(2, error_msg, ft_strlen(error_msg));
}

void	display_err_with_prefix(const char *prefix, const char *error_msg)
{
	if (!error_msg)
		write(2, "Error\n", 6);
	if (prefix)
		write(2, prefix, ft_strlen(prefix));
	write(2, ":", 1);
	if (error_msg)
		write(2, error_msg, ft_strlen(error_msg));
}

void	display_syntax_error(const char c)
{
	display_error("Syntax error near unexpected token `");
	write(2, &c, 1);
	write(2, "'\n", 2);
}

char	*ft_strjoin_with_free(char *s1, char *s2)
{
	char	*result;
	size_t	s1_len;
	size_t	s2_len;

	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	result = (char *) malloc((s1_len + s2_len + 1) * sizeof(char));
	if (!result)
		return (NULL);
	if (s1_len == 0 && s2_len == 0)
		result[0] = '\0';
	else
	{
		if (s1_len > 0)
		{
			ft_strlcpy(result, s1, s1_len + 1);
			free(s1);
		}
		if (s2_len > 0)
		{
			ft_strlcpy(result + s1_len, s2, s2_len + 1);
			free(s2);
		}
	}
	return (result);
}

int	ends_with_a_closing_brace(t_token *t, int j)
{
	while (j < t->src_len && t->src[j] && t->src[j] != '}')
		j++;
	if (!(j < t->src_len && t->src[j] && t->src[j] == '}'))
		return (0);
	return (1);
}
