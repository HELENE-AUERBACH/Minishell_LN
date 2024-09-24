/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_parser_2_minishell.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmorice <rmorice@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 18:28:07 by hauerbac          #+#    #+#             */
/*   Updated: 2024/09/24 14:21:22 by rmorice          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_minishell.h"

/* ************************************************************************** */
/*                                del_content                                 */
/* -------------------------------------------------------------------------- */
/* This function frees content and make it points toward NULL.                */
/* Input :                                                                    */
/*  - void *content : the element to free and replaces by NULL                */
/* Return :                                                                   */
/*  - None                                                                    */
/* ************************************************************************** */
static void	del_content(void *content)
{
	if (content)
	{
		free(content);
		content = NULL;
	}
	return ;
}

/* ************************************************************************** */
/*                         add_or_concatenate_substr                          */
/* -------------------------------------------------------------------------- */
/* This function adds or concatenates the sub-string sstr at the end of the   */
/* list lst. In case of concatenation the substring is joined to the last     */
/* content of lst.                                                            */
/* The add or the concatenation depends of the valu of d[END]. If d[END] == 0 */
/* then the substring is add at the back of the list, otherwise it is joined  */
/* to lst's last node content                                                 */
/* Inputs :                                                                   */
/*  - t_list **lst      */
/*  - char *sstr : the sub-string to add at the end of the previous one       */
/*  - int *d : array of datas about internal criteria of token                */
/*  - int *nb_substrs : a pointer to number of sub-strings contained in lst   */
/* Return :                                                                   */
/*  - 0 : if everything goes well                                             */
/*  - -3 : otherwise (malloc error)                                           */
/* ************************************************************************** */
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

/* ************************************************************************** */
/*                     add_new_substr_to_list_of_cmd_args                     */
/* -------------------------------------------------------------------------- */
/* This function creates a substring that is a copy of src from index d[I] to */
/* d[J]. Then this substring is added to lst (list of commands arguments ???) */
/* Then, the index d[I] is update to the first index after the end of the     */
/* sub-string if the substring was quoted or if src[d[J]] is a space.         */
/* Otherwise the inde d[J] become the new index d[I]                          */
/* the index d[END] is also update (decreased by 1) if the new d[I] does not  */
/* point toward a space in src                                                */
/* Inputs :                                                                   */
/*  - t_list **lst      */
/*  - char *src : the string that contained the command line                  */
/*  - int *d : array of datas about internal criteria of token                */
/*  - int *nb_substrs : a pointer to number of sub-strings contained in lst   */
/* Return :                                                                   */
/*  - 0 : if everything goes well                                             */
/*  - int : otherwise                                                         */
/* ************************************************************************** */
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
		if (d[I] < d[STR_LEN] && !is_a_space(src[d[I]]))
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

/* ************************************************************************** */
/*                          extract_list_of_cmd_args                          */
/* -------------------------------------------------------------------------- */
/* This function updates the index of start and end of the token and add the  */
/* sub-string obtained at the end of the lst of command and arguments         */
/* To do so it increment by one d[I] if the first char is an opening quote or */
/* by as many consecutives spaces their is at the beginning of src (starting  */
/* with d[I] index)                                                           */
/* Then d[J] is incremented until the first unquoted space is encounter or a  */
/* closing parenthesis is encountered                                         */
/* We then used these indexes to create a sub-string that is added at the end */
/* of lst. The value of d[I], d[J] and d[End] is update if needed             */
/* we put to 0 booleans used to determined if we have met an opening quote    */
/* Inputs :                                                                   */
/*  - t_list **lst      */
/*  - char *src : the string that contained the command line                  */
/*  - int *d : array of datas about internal criteria of token                */
/*  - int *nb_substrs : a pointer to number of sub-strings contained in lst   */
/* Return :                                                                   */
/*  - 0 : if everything goes well                                             */
/*  - int : otherwise                                                         */
/* ************************************************************************** */
// we increment d[I] then we look at d[J] but how are we sure that d[J] < d[I]
// even after the d[I] incrementation ???
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

/* ************************************************************************** */
/*                      copy_list_into_tab_and_free_list                      */
/* -------------------------------------------------------------------------- */
/* This function copies each and every elements of lst into the array of      */
/* string "tab". Each node is then free                                       */
/* if no sub-string have been created we clear the list lst                   */
/* Inputs :                                                                   */
/*  - char **tab      */
/*  - t_list **lst      */
/*  - int nb_substrs : the number of sub-strings contained in lst             */
/* Return :                                                                   */
/*  - 0 : if everything goes well                                             */
/*  - int : otherwise                                                         */
/* ************************************************************************** */
// hmm... why don't we need to specified the length of each strings contained
// in the array of str ???
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
