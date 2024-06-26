# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jbocktor <jbocktor@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/12 17:04:21 by hauerbac          #+#    #+#              #
#    Updated: 2024/06/06 13:44:57 by hauerbac         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CFLAGS = -Wall -Wextra -Werror 
DEPSFLAG = -MMD

DEBUG := 0
ifeq ($(DEBUG), 1)
	CFLAGS += -g3
endif

SRCS_DIR = srcs

vpath %.c ${SRCS_DIR}

SRCS =	free_2_minishell.c \
	free_minishell.c \
	ctypes_split_lexer_minishell.c \
	checks_split_lexer_minishell.c \
	extract_lexer_minishell.c \
	split_lexer_minishell.c \
	ctypes_tokens_lexer_minishell.c \
	lexer_minishell.c \
	signals_handlers_minishell.c \
	utils_minishell.c \
	environment_minishell.c \
	perform_expansions_2_lexer_minishell.c \
	perform_expansions_lexer_minishell.c \
	heredoc_files_management_2_minishell.c \
	heredoc_files_management_minishell.c \
	files_management_2_minishell.c \
	files_management_3_minishell.c \
	files_management_minishell.c \
	parser_tokens_2_minishell.c \
	split_parser_2_minishell.c \
	split_parser_minishell.c \
	split_path_env_var_minishell.c \
	parser_tokens_3_minishell.c \
	close_descriptors_minishell.c \
	built_cd.c\
	built_cd_2.c\
	built_echo.c\
	built_env.c\
	utils_3_minishell.c \
	utils_2_minishell.c \
	built_exit.c\
	built_export.c\
	built_export_2.c\
	built_export_3.c\
	built_pwd.c\
	built_unset.c\
	run_bi_minishell.c \
	run_cmds_3_minishell.c \
	run_cmds_2_minishell.c \
	run_cmds_minishell.c \
	parser_tokens_4_minishell.c \
	parser_tokens_minishell.c \
	debug_minishell.c \
	process_interactive_mode_minishell.c \
	process_non_interactive_mode_minishell.c \
	copy_environment_minishell.c \
	minishell.c

HEADERS =	includes/lexer_minishell.h \
		includes/minishell.h

BUILD_DIR = .build
OBJS = $(addprefix $(BUILD_DIR)/, $(SRCS:%.c=%.o))
DEPS = $(addprefix $(BUILD_DIR)/, $(SRCS:%.c=%.d))

LIBFT_DIR = libft

LIBFT = ${LIBFT_DIR}/libft.a

NAME = minishell

all : LIBRARY BUILD ${NAME}

LIBRARY:
	make -C ${LIBFT_DIR} all

BUILD:
	mkdir -p ${BUILD_DIR}

clean:
	make -C ${LIBFT_DIR} clean
	rm -rf ${BUILD_DIR}

fclean: clean
	rm -f ${NAME}
	rm -f a.out
	rm -f ${LIBFT}

re: fclean all

-include ${DEPS}

${BUILD_DIR}/%.o : %.c ${HEADERS} Makefile
	cc ${CFLAGS} ${DEPSFLAG} -I./includes -c $< -o $@

${NAME}: ${OBJS} ${LIBFT}
	cc ${CFLAGS} ${DEPSFLAG} -o ${NAME} ${OBJS} -lreadline -L${LIBFT_DIR} -lft

.PHONY: all clean fclean re
