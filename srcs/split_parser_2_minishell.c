/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_parser_2_minishell.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 18:28:07 by hauerbac          #+#    #+#             */
/*   Updated: 2024/05/03 21:10:20 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_minishell.h"

static void	del_content(void *content)
{
	if (content)
	{
		free(content);
		content = NULL;
	}
	return ;
}

static int	add_or_concatenate_substr(t_list **lst, char *sstr, int *d,
			int *nb_substrs)
{
	t_list	*lst_new;
	char	*previous_substr;

	if (d[END] == 0)
	{
		lst_new = ft_lstnew((void *) sstr);
		if (!lst_new)
			return (-3);
		ft_lstadd_back(lst, lst_new);
		(*nb_substrs)++;
	}
	else
	{
		lst_new = *lst;
		while (lst_new && lst_new->next)
			lst_new = lst_new->next;
		previous_substr = (char *) lst_new->content;
		lst_new->content = (void *) \
			ft_strjoin(previous_substr, sstr);
		free(previous_substr);
		free(sstr);
		if (!lst_new->content)
			return (-3);
	}
	return (0);
}

static int	add_new_substr_to_list_of_cmd_args(t_list **lst, char *src,
			int *d, int *nb_substrs)
{
	char	*substr;

	substr = (char *) malloc((d[J] - d[I] + 1) * sizeof(char));
	if (!substr)
		return (ft_lstclear(lst, &del_content), -3);
	ft_strlcpy(substr, src + d[I], d[J] - d[I] + 1);
	if (add_or_concatenate_substr(lst, substr, d, nb_substrs) < 0)
		return (ft_lstclear(lst, &del_content), -3);
	d[END] = 0;
	if (d[EXT_OPEN_S_QUOTE] == 1 || d[EXT_OPEN_D_QUOTE] == 1)
	{
		d[I] = d[J] + 1;
		if (!is_a_space(src[d[I]]))
			d[END] = -1;
	}
	else if (is_a_space(src[d[J]]))
		d[I] = d[J] + 1;
	else
	{
		d[I] = d[J];
		if (!is_a_space(src[d[I]]))
			d[END] = -1;
	}
	return (0);
}

int	extract_list_of_cmd_args(t_list **lst, char *src, int *d,
		int *nb_substrs)
{
	while (d[I] < d[STR_LEN])
	{
		d[J] = d[I] + 1;
		check_simple_quotes(d[I], d, src);
		check_double_quotes(d[I], d, src);
		if (d[EXT_OPEN_S_QUOTE] == 1 || d[EXT_OPEN_D_QUOTE] == 1)
			d[I]++;
		else
			while (d[I] < d[STR_LEN] && is_a_space(src[d[I]]))
				d[I]++;
		while (d[J] < d[STR_LEN] && ((d[EXT_OPEN_S_QUOTE] == 0 \
			&& d[EXT_OPEN_D_QUOTE] == 0 && !is_a_space(src[d[J]]) \
			&& src[d[J]] != '\'' && src[d[J]] != '"') \
			|| (d[EXT_OPEN_S_QUOTE] == 1 && src[d[J]] != '\'') \
			|| (d[EXT_OPEN_D_QUOTE] == 1 && src[d[J]] != '"')))
			d[J]++;
		if (d[I] < d[STR_LEN] && d[J] <= d[STR_LEN] && d[I] < d[J] \
			&& add_new_substr_to_list_of_cmd_args(lst, src, d,
				nb_substrs) < 0)
			return (display_error("Malloc error for cmd args\n"), \
						-3);
		d[EXT_OPEN_S_QUOTE] = 0;
		d[EXT_OPEN_D_QUOTE] = 0;
	}
	return (0);
}

void	copy_list_into_tab_and_free_list(char **tab, t_list **lst,
		int nb_substrs)
{
	int		i;
	t_list	*current;

	if (lst && *lst)
	{
		if (tab && nb_substrs > 0)
		{
			i = 0;
			current = *lst;
			while (current && i < nb_substrs)
			{
				tab[i] = current->content;
				current = current->next;
				free(*lst);
				*lst = current;
				i++;
			}
		}
		else
			ft_lstclear(lst, &del_content);
	}
}
