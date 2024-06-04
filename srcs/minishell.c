/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 16:38:35 by hauerbac          #+#    #+#             */
/*   Updated: 2024/06/04 16:43:36 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	init_minishell_data(t_data *d)
{
	g_exit_status = 0;
	if (!d)
		return (EXIT_FAILURE);
	d->lst = NULL;
	d->cmds = NULL;
	d->nb_cmds = -1;
	d->return_code = 0;
	d->new_files = NULL;
	d->cmd_new_files = NULL;
	d->envp = NULL;
	d->envp_size = -1;
	return (0);
}

int	is_in_interactive_mode(void)
{
	return (isatty(STDIN_FILENO) && isatty(STDERR_FILENO));
}

int	g_exit_status;

int	main(const int argc, char **argv, char **envp)
{
	int		fd;
	int		result;
	t_data	d;

	init_minishell_data(&d);
	copy_environment(&d, envp);
	if (d.envp == NULL)
		return (display_error("Environment copy error\n"), 1);
	if (is_in_interactive_mode() && argc == 1)
		return (process_interactive_mode(&d));
	fd = 0;
	if (argc == 2)
	{
		fd = open(argv[1], O_RDONLY);
		if (fd == -1)
			return (perror("Input file open"), free_data(&d), 1);
	}
	result = 1;
	if (argc <= 2)
		result = process_non_interactive_mode(fd, &d);
	else if (argc == 3 && argv[1] && argv[1][0] == '-' && argv[1][1] == 'c')
		return (process_non_interactive_mode_with_c_opt(argv[2], &d));
	if (fd != 0 && close(fd) == -1)
		return (perror("Input file close"), free_data(&d), 1);
	return (free_data(&d), result);
}
