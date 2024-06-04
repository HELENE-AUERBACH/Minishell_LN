/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 14:07:23 by hauerbac          #+#    #+#             */
/*   Updated: 2024/06/04 12:05:16 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H

# define LIBFT_H

# include <stddef.h>
# include <stdlib.h>

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}					t_list;

typedef struct s_dll_el
{
	void			*content;
	struct s_dll_el	*prev;
	struct s_dll_el	*next;
}					t_dll_el;

typedef struct s_dll
{
	size_t			size;
	struct s_dll_el	*head;
	struct s_dll_el	*tail;
}				t_dll;

int			ft_isalpha(int c);
int			ft_isdigit(int c);
int			ft_isalnum(int c);
size_t		ft_strlen(const char *s);
void		*ft_memset(void *s, int c, size_t n);
void		ft_bzero(void *s, size_t n);
size_t		ft_strlcpy(char *dst, const char *src, size_t size);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
int			ft_atoi(const char *nptr);
char		*ft_strdup(const char *s);
char		*ft_substr(char const *s, unsigned int start, size_t len);
char		*ft_strjoin(char const *s1, char const *s2);
char		*ft_strtrim(char const *s1, char const *set);
char		*ft_itoa(int n);
void		ft_putchar_fd(char c, int fd);
void		ft_putstr_fd(char *s, int fd);
void		ft_putendl_fd(char *s, int fd);
void		ft_putnbr_fd(int n, int fd);
t_list		*ft_lstnew(void *content);
void		ft_lstadd_front(t_list **lst, t_list *new);
int			ft_lstsize(t_list *lst);
t_list		*ft_lstlast(t_list *lst);
void		ft_lstadd_back(t_list **lst, t_list *new);
void		ft_lstdelone(t_list *lst, void (*del)(void *));
void		ft_lstclear(t_list **lst, void (*del)(void *));
void		ft_lstiter(t_list *lst, void (*f)(void *));
t_list		*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));
t_dll		*new_empty_dll(void);
t_dll_el	*new_dll_element(void *content);
int			insert_before(t_dll *lst, t_dll_el *ptr, void *content);
int			insert_at_dll_head(t_dll *lst, void *content);
int			insert_at_dll_tail(t_dll *lst, void *content);
int			insert_after(t_dll *lst, t_dll_el *ptr, void *content);
void		del_el_at_dll_head(t_dll *lst, void (*del)(void *));
void		del_el_at_dll_tail(t_dll *lst, void (*del)(void *));
void		delete_dll_el(t_dll *lst, t_dll_el *ptr, void (*del)(void *));
void		empty_dll(t_dll *lst, void (*del)(void *));
void		empty_dll_before_cur(t_dll *lst, t_list *current,
				void (*del)(void *));
void		empty_dll_after_cur(t_dll *lst, t_list *current,
				void (*del)(void *));
void		dll_clear(t_dll **lst, void (*del)(void *));
int			dll_iter(t_dll *lst, int (*f)(t_dll_el *));
int			dll_iter_with_param(t_dll *lst, void *param,
				int (*f)(t_dll_el *, void *));
void		print_dll(t_dll *lst, int fd, void (*f)(int, void *));
t_dll		*dll_map(t_dll *lst, void *(*f)(void *), void (*del)(void *));
t_dll_el	*get_last_spec_ptr_from_tail(t_dll *lst, int (*f)(void *));
t_dll_el	*get_first_spec_ptr_from_tail(t_dll *lst, int (*f)(void *));

#endif
