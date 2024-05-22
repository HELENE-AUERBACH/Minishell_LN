# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/08 18:20:11 by hauerbac          #+#    #+#              #
#    Updated: 2024/05/21 14:26:05 by hauerbac         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CFLAGS = -Wall -Wextra -Werror
DEPSFLAG = -MMD

DEBUG := 0
ifeq ($(DEBUG), 1)
	CFLAGS += -g3
endif

SRCS =  sources/strings/ft_strlen.c \
		sources/memory/ft_memset.c \
		sources/memory/ft_bzero.c \
		sources/strings/ft_strlcpy.c \
		sources/strings/ft_strncmp.c \
		sources/conversion/ft_atoi.c \
		sources/strings/ft_strdup.c \
		sources/strings/ft_substr.c \
		sources/strings/ft_strjoin.c \
		sources/strings/ft_strtrim.c \
		sources/conversion/ft_itoa.c \
		sources/output/ft_putchar_fd.c \
		sources/output/ft_putstr_fd.c \
		sources/output/ft_putendl_fd.c \
		sources/output/ft_putnbr_fd.c \
		sources/list/ft_lstnew.c \
		sources/list/ft_lstadd_front.c \
		sources/list/ft_lstsize.c \
		sources/list/ft_lstlast.c \
		sources/list/ft_lstadd_back.c \
		sources/list/ft_lstdelone.c \
		sources/list/ft_lstclear.c \
		sources/list/ft_lstiter.c \
		sources/list/ft_lstmap.c \
		sources/dll/new_empty_dll.c \
		sources/dll/new_dll_element.c \
		sources/dll/insert_before.c \
		sources/dll/insert_at_dll_head.c \
		sources/dll/insert_at_dll_tail.c \
		sources/dll/insert_after.c \
		sources/dll/del_el_at_dll_head.c \
		sources/dll/del_el_at_dll_tail.c \
		sources/dll/delete_dll_el.c \
		sources/dll/empty_dll.c \
		sources/dll/empty_dll_before_cur.c \
		sources/dll/empty_dll_after_cur.c \
		sources/dll/dll_clear.c \
		sources/dll/dll_iter.c \
		sources/dll/dll_iter_with_param.c \
		sources/dll/print_dll.c \
		sources/dll/dll_map.c \
		sources/dll/get_last_spec_ptr_from_tail.c \
		sources/dll/get_first_spec_ptr_from_tail.c

HEADERS =	includes/libft.h

OBJS = ${SRCS:.c=.o}
DEPS = ${SRCS:.c=.d}

NAME = libft.a

all : ${NAME}

clean:
	rm -f ${OBJS}
	rm -f ${DEPS}

fclean: clean
	rm -f ${NAME}
	rm -f a.out

re: fclean all

-include ${DEPS}

%.o : %.c ${HEADERS} Makefile
	cc ${CFLAGS} ${DEPSFLAG} -I./includes -c $< -o $@

${NAME}: ${OBJS}
	ar rc ${NAME} ${OBJS}
	ranlib ${NAME}
