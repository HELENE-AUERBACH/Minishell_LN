/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_cmds_2_minishell.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 14:17:39 by hauerbac          #+#    #+#             */
/*   Updated: 2024/05/17 14:50:48 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_signals_actions_in_fork(void)
{
	struct sigaction	s1;
	struct sigaction	s2;

	s1.sa_handler = &sigint_handler_in_fork;
	sigemptyset(&s1.sa_mask);
	s1.sa_flags = SA_RESTART;
	sigaction(SIGINT, &s1, NULL);
	s2.sa_handler = &sigquit_handler_in_fork;
	sigemptyset(&s2.sa_mask);
	s2.sa_flags = SA_RESTART;
	sigaction(SIGQUIT, &s2, NULL);
}

static int	join_path_with_cmd(t_cmd *cmd_d, char *command,
			char *command_with_path2)
{
	if (command_with_path2 == NULL)
		return (-4);
	if (access(command_with_path2, X_OK) == 0)
	{
		free(command);
		cmd_d->cmd = command_with_path2;
		return (0);
	}
	return (-1);
}

static int	check_command(t_cmd *cmd_d, char *command)
{
	int		i;
	char	*cmd_with_path1;
	char	*cmdp2;

	if (command && (command[0] == '.' || command[0] == '/') && command[1])
		return (free_tab(&cmd_d->paths_tab), access(command, X_OK));
	if (!cmd_d->paths_tab || command == NULL || (command[0] && !command[1]))
		return (free_tab(&cmd_d->paths_tab), -2);
	i = 0;
	while (cmd_d->paths_tab[i])
	{
		if (cmd_d->paths_tab[i][0] != '\0')
			cmd_with_path1 = ft_strjoin(cmd_d->paths_tab[i], "/");
		else
			cmd_with_path1 = ft_strjoin(".", "/");
		if (cmd_with_path1 == NULL)
			return (free_tab(&cmd_d->paths_tab), -3);
		cmdp2 = ft_strjoin(cmd_with_path1, command);
		free(cmd_with_path1);
		if (join_path_with_cmd(cmd_d, command, cmdp2) == 0)
			return (free_tab(&cmd_d->paths_tab), 0);
		free(cmdp2);
		i++;
	}
	return (free_tab(&cmd_d->paths_tab), -5);
}

static void	check_command_and_find_path(t_data *d, t_token *t, int ds[3],
			int is_piped)
{
	char	*trimmed_cmd;

	trimmed_cmd = NULL;
	if (t && t->cmd_d && t->cmd_d->cmd)
		trimmed_cmd = ft_strtrim(t->cmd_d->cmd, " ");
	if (!(trimmed_cmd && ft_strlen(trimmed_cmd) > 0 \
		&& check_command(t->cmd_d, t->cmd_d->cmd) == 0))
	{
		if (trimmed_cmd)
			free(trimmed_cmd);
		trimmed_cmd = NULL;
		display_err_with_prefix(t->cmd_d->cmd, " command not found\n");
		close_descrs_with_a_possible_exit(d, t, ds, is_piped);
		empty_list(&d->cmds);
		empty_list(&d->new_files);
		empty_list(&d->cmd_new_files);
		if (is_in_interactive_mode())
			rl_clear_history();
		free_data(d);
		exit (127);
	}
	free(trimmed_cmd);
	trimmed_cmd = NULL;
}

void	run_command(t_data *d, t_token *t, int ds[3])
{
	const int	is_piped = (ds[0] != -1 && ds[1] != -1);

	t->cmd_d->pid = fork();
	if ((int) t->cmd_d->pid == -1)
		perr_cds(d, "Fork error", ds, is_piped);
	if ((int) t->cmd_d->pid == 0)
	{
		set_signals_actions_in_fork();
		if (t->cmd_d->is_in_piped == 1 && ds[2] != -1
			&& t->cmd_d->fd1 == -1 && dup2(ds[2], 0) == -1)
			perr_cds(d, DUP_PREV_ERR, ds, is_piped);
		if (t->cmd_d->fd1 != -1 && dup2(t->cmd_d->fd1, 0) == -1)
			perr_cds(d, REDIR_INPUT_ERR, ds, is_piped);
		if (t->cmd_d->is_out_piped == 1 && t->cmd_d->fd2 == -1
			&& dup2(ds[1], 1) == -1)
			perr_cds(d, DUP_1_ERR, ds, is_piped);
		if (t->cmd_d->fd2 != -1 && dup2(t->cmd_d->fd2, 1) == -1)
			perr_cds(d, REDIR_OUTPUT_ERR, ds, is_piped);
		check_command_and_find_path(d, t, ds, is_piped);
		close_descrs_with_a_possible_exit(d, t, ds, is_piped);
		execve(t->cmd_d->cmd, t->cmd_d->args, d->envp);
		perr_cds(d, "Exec command failed error", ds, is_piped);
	}
	close_ds_in_parent(t, ds, is_piped);
}
