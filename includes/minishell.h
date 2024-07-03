/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 16:49:01 by hauerbac          #+#    #+#             */
/*   Updated: 2024/07/03 14:06:00 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H

# define MINISHELL_H

# ifndef BUFFER_SIZE

#  define BUFFER_SIZE 4242

# endif

# define CLOSE_0_ERR "close_descriptors-Pipe descriptor 0 close error"
# define CLOSE_1_ERR "close_descriptors-Pipe descriptor 1 close error"
# define CLOSE_PREV_ERR "close_descriptors-Previous pipe descr. 0 close error"
# define CLOSE_INPUT_ERR "close_descriptors-Input file close error"
# define CLOSE_OUTPUT_ERR "close_descriptors-Output file close error"
# define REDIR_INPUT_ERR "In fork-Child-Input file redirection error"
# define DUP_PREV_ERR "In fork-Child-Previous pipe descriptor 0 dup2 error"
# define DUP_1_ERR "In fork-Child-Pipe descriptor 1 dup2 error"
# define REDIR_OUTPUT_ERR "In fork-Child-Output file redirection error"
# define PARENT_CLOSE_IN_ERR "In fork-Parent-Input file close error"
# define PARENT_CLOSE_PREV_ERR "In fork-Parent-Prev. pipe descr. 0 close error"
# define PARENT_CLOSE_1_ERR "In fork-Parent-Pipe descriptor 1 close error"
# define PARENT_CLOSE_OUT_ERR "In fork-Parent-Output file close error"
# define BI_REDIR_IN_ERR "run_bi_without_fork-Input file redirection error"
# define BI_REDIR_OUT_ERR "run_bi_without_fork-Output file redirection error"
# define WRONG_CWD_1 "chdir: error retrieving current directory"
# define WRONG_CWD_2 " getcwd: cannot access parent directories"

# include "lexer_minishell.h"
# include <sys/stat.h>

typedef struct s_data
{
	char		**envp;
	t_dll		*lst;
	t_list		*cmds;
	t_list		*new_files;
	t_list		*cmd_new_files;
	int			envp_size;
	int			nb_cmds;
	long long	return_code;
}				t_data;

int		is_in_interactive_mode(void);
void	perror_and_code(int *result, const char *s, int new_result);
int		close_descriptors(int pipedescr[3], int is_piped);
int		perr_cds(t_data *d, const char *s, int pipedescriptors[3],
			int is_piped);
void	close_descrs_with_a_possible_exit(t_data *d, t_token *t,
			int pipedescr[3], int is_piped);
void	close_ds_in_parent(t_token *t, int ds[3], int is_piped);
void	copy_pwd_var_into_env(char **envp, int envp_size);
void	copy_environment(t_data *d, char **envp);
void	free_tab_with_size(char ***tab, int size);
void	free_cmd_d_without_unlink(t_cmd *cmd_d);
void	close_files_and_free_files_names_without_unlink(t_cmd *cmd_d);
void	del_el_content(void *content);
void	free_tab(char ***tab);
void	free_data(t_data *d);
int		get_value_from_envp(char **value, char *name, int envp_size,
			char **envp);
int		get_value_from_minishell_envp(char **value, char *name,
			t_data *minishell_data);
int		is_a_pipe_or_ctrloperator(enum e_token_types type);
t_token	*cast_dll_el_into_token(t_dll_el **current);
int		open_file(t_list **new_files, t_list **cmd_new_files,
			t_token *t, char *file_name);
int		write_here_doc_file(t_data *d, t_cmd *cmd_d, t_list *cur);
int		create_heredoc_file(t_data *d, t_token *t, t_dll_el *prev);
int		check_files_for_in_redirections(t_dll_el **current,
			int redir_error_first_position, t_data *d,
			int is_in_piped);
int		check_redir_files(t_dll_el **current, t_data *d,
			int is_in_piped);
int		set_last_redir_files_names(t_token *t_cmdbi,
			t_dll_el *start_el);
int		add_to_cmds_list(t_data *d, t_token *t_cmdbi);
void	force_in_redir_of_next_cmdbi(t_token *t_cmdbi,
			int *next_has_to_be_forced, int *result);
int		parse_tokens(t_data *d);
int		files_open(t_token *t);
int		check_and_run_bi(t_data *d, t_cmd *bi, int fd2);
int		run_bi_without_fork(t_data *d, t_token *t);
void	set_signals_actions_in_fork(t_dll *lst, t_list *current);
int		updenv(char ***envp, int *envp_size, char *var_name,
			char *var_value);
int		built_cd(char ***envp, int *envp_size, char **args, int fd);
int		built_echo(char **args, int fd);
int		built_env(char **envp, char **args, int fd);
int		built_exit(t_data *d, char **args);
int		var_name_found_in_env(char *envp_at_idx_i, char *var_name,
			size_t var_name_len);
int		update_envp_with_var(char ***envp, int *envp_size, char *var,
			int i);
int		built_export(char ***envp, int *envp_size, char **args,
			int fd);
int		built_pwd(char **args, int fd);
int		built_unset(char ***envp, int *envp_size, char **args);
void	run_bi_in_fork(t_data *d, t_token *t, int ds[3], t_list *current);
int		check_error_on_command(char *cmd);
void	run_command(t_data *d, t_token *t, int ds[3], t_list *current);
int		run_commands(t_data *d);
void	empty_list(t_list **lst);
int		process_a_line(char *a_line, t_data *d);
int		process_interactive_mode(t_data *d);
int		process_non_interactive_mode(int fd, t_data *d);
int		process_non_interactive_mode_with_c_opt(char *an_argv,
			t_data *d);

#endif
