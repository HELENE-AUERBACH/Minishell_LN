/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_parser_minishell.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 15:46:15 by hauerbac          #+#    #+#             */
/*   Updated: 2024/05/03 21:31:51 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_minishell.h"

char	**split_cmd_args(t_token *t)
{
	int		nb_substrs;
	t_list	*lst;
	int		d[10];
	int		i;
	char	**tab;

	nb_substrs = 0;
	tab = NULL;
	init_utils_data(d, t->src_len);
	if (!(d[I] < d[STR_LEN]))
		return (tab);
	lst = NULL;
	if (extract_list_of_cmd_args(&lst, t->src, d, &nb_substrs) < 0)
		return (NULL);
	if (nb_substrs > 0)
	{
		tab = (char **) malloc(sizeof(char *) * (nb_substrs + 1));
		if (!tab)
			return (NULL);
		i = 0;
		while (i < nb_substrs + 1)
			tab[i++] = NULL;
		copy_list_into_tab_and_free_list(tab, &lst, nb_substrs);
	}
	return (tab);
}
