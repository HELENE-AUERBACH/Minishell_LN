/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_minishell.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmorice <rmorice@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 14:07:38 by hauerbac          #+#    #+#             */
/*   Updated: 2024/09/25 10:19:41 by rmorice          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_MINISHELL_H

# define LEXER_MINISHELL_H

# ifndef HEREDOC_BUFFER_SIZE

#  define HEREDOC_BUFFER_SIZE 4242

# endif

# include "../libft/includes/libft.h"
# include <sys/types.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <stdio.h>
# include <errno.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <limits.h>

extern int	g_exit_status;

enum e_tokenizer_data
{
	STR_LEN,
	START,
	END,
	EXT_OPEN_D_QUOTE,
	EXT_OPEN_S_QUOTE,
	EXT_OPENING_PARENTHESIS,
	EXT_CLOSING_PARENTHESIS,
	I,
	J,
	HAS_EXPANSIONS,
};

typedef struct s_tokenizer_data
{
	int			i;
	int			j;
	int			position;
	int			data[9];
	char		*raw_command;
	t_dll		*lst;
	t_dll_el	*dll_current_el_ptr;
}		t_tokenizer_data;

typedef struct s_cmd
{
	char	*file1;
	char	*file2;
	char	*limiter;
	char	*cmd;
	char	**paths_tab;
	char	**args;
	pid_t	pid;
	int		fd1;
	int		fd2;
	int		is_here_doc;
	int		is_fd2_o_append;
	int		is_in_piped;
	int		is_out_piped;
	int		followed_by_op;
	int		order;
}				t_cmd;

enum e_token_types
{
	UNKNOWN,
	S_IN_REDI,
	S_OUT_REDI,
	HEREDOC,
	D_OUT_REDI,
	COMMAND,
	BI,
	PIPE,
	SEMICOLON,
	AMPERSAND,
	DOUBLE_SEMICOLON,
	LOGICAL_AND,
	LOGICAL_OR,
	SEMICOLON_AMPERSAND,
	DOUBLE_SEMICOLON_AMPERSAND,
	PIPE_AMPERSAND,
	OPENING_PARENTHESIS,
	CLOSING_PARENTHESIS,
};

typedef struct s_token
{
	enum e_token_types	type;
	int					position;
	int					src_len;
	char				*src;
	t_cmd				*cmd_d;
}		t_token;

// utils_minishell.c
void				display_error(const char *error_msg);
void				display_err_with_prefix(const char *prefix,
						const char *error_msg);
void				display_syntax_error(const char c);

// utils_2_minishell.c
void				display_err_with_2_prefixes(const char *prefix1,
						const char *prefix2,
						const char *error_msg);

// signals_handlers_minishell.c
void				sigint_handler_in_main(int signo);
void				sigint_handler_in_fork(int signo);
void				sigquit_handler_in_fork(int signo);
void				heredoc_handler_in_main(int signo);

// process_interactive_mode_minishell.c
void				set_signals_actions(void);

// heredoc_files_management_minishell.c
void				close_in_file_and_free_file_name(t_cmd *cmd_d);
void				close_out_file_and_free_file_name(t_cmd *cmd_d);

// utils_minishell.c
char				*ft_strjoin_with_free(char *s1, char *s2);

// run_cmds_3_minishell.c
char				*ft_strjoin_with_free_s1(char *s1, char *s2);

// ctypes_split_lexer_minishell.c
int					is_a_space(const char c);
int					is_a_metacharacter(const char c);
int					is_a_command_separator(const char c);
int					is_a_redirection(int i, int *data,
						const char *str);
int					is_a_char_of_raw_command(int i,
						int *data, const char *str);

// checks_split_lexer_minishell.c
void				check_double_quotes(int i, int *data,
						const char *str);
void				check_simple_quotes(int i, int *data,
						const char *str);
void				check_parenthesis(int i, int *data,
						const char *str);
int					has_wrong_start(const char c,
						const char c2);
int					has_wrong_format(int *data,
						const char *str);

// lexer_minishell.c
int					get_end_index_of_file_or_delimiter_name(
						int *i, const char *str);

// split_lexer_minishell.c
int					end_raw_command(t_tokenizer_data *d);

// ctypes_tokens_lexer_minishell.c
int					is_a_cmdbi_or_outredirection_dll_el(
						void *el_content);
int					is_a_cmd_or_bi_dll_el(
						void *el_content);
int					is_an_out_redirect(void *el_content);
int					is_an_in_redirect(void *el_content);

// lexer_minishell.c
int					init_token_cmd_d(t_token *token);
int					init_token(int type, char *src,
						t_tokenizer_data *d);

// extract_lexer_minishell.c
int					extract_tokens_into_dll(
						t_tokenizer_data *d,
						const char *str);

// debug_minishell.c (not needed outside of tests and debug)
void				print_dll_el_content(int fd, void *el_content);

// split_lexer_minishell.c
int					split_in_tokens(const char *str,
						t_dll *lst);

// perform_expansions_lexer_minishell.c
void				init_utils_data(int *utils_data, int str_len);
int					is_a_special_char(const char c);

// utils_minishell.c
int					ends_with_a_closing_brace(t_token *t,
						int j);

// perform_expansions_2_lexer_minishell.c
int					is_not_a_valid_identifier(t_token *t,
						int i, int j, char **new_src);
int					get_new_src_for_expansion(
						char **new_src, t_token *t,
						int *d, void *param);

// perform_expansions_lexer_minishell.c
int					perform_one_expansion(char **new_src,
						int *d, t_token *t,
						void *param);

// perform_expansions_2_lexer_minishell.c
int					loop_perform_expansions(char **new_src,
						int *d, t_token *t,
						void *param);

// lexer_minishell.c
void				init_utils_data_and_new_src(int *utils_data,
						char **new_src, int str_len);

// perform_expansions_lexer_minishell.c
int					perform_expansions(t_dll_el *el_ptr,
						void *param);

// free_minishell.c
void				free_cmd_d(t_cmd **cmd_d);

// parser_tokens_2_minishell.c
int					remove_first_spaces_and_ext_quotes(
						t_token *t);

// files_management_2_minishell.c
int					close_previous_file(t_dll_el *prev);
int					check_out_redir_file(t_token *t);
int					check_in_redir_file(t_token *t,
						t_list *new_files,
						t_list *cmd_new_files,
						int is_in_piped);

// files_management_3_minishell.c
int					check_files_for_out_redirections(
						t_dll_el **current,
						t_list **new_files,
						t_list **cmd_new_files,
						int redir_error_first_position);

// split_parser_2_minishell.c
int					extract_list_of_cmd_args(t_list **lst,
						char *src, int *d,
						int *nb_substrs);
void				copy_list_into_tab_and_free_list(char **tab,
						t_list **lst, int nb_substrs);

// split_parser_minishell.c
char				**split_cmd_args(t_token *t);

// split_path_env_var_minishell.c
char				**get_paths(char **envp);

// utils_3_minishell.c
unsigned int		ft_spaces_truncating(const char *str);
unsigned int		ft_chartodigit(const char c);

// utils_2_minishell.c
unsigned long long	ft_strtoll(const char *nptr, char **endptr, int *sign);

#endif
