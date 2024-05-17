/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_bi_minishell.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hauerbac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 15:56:35 by hauerbac          #+#    #+#             */
/*   Updated: 2024/05/16 16:33:27 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	start_n(const char *arg)
{
	size_t	i;

	i = 2;
	if (ft_strncmp(arg, "-n", ft_strlen("-n")) != 0)
		return (0);
	while (i < ft_strlen(arg))
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

static int	built_echo(char **echo)
{
	int		i;
	int		new_line;
	char	**echo_string;

	echo_string = &echo[1];
	new_line = 1;
	i = 0;
	while (echo_string[i] != NULL && start_n(echo_string[i]))
	{
		new_line = 0;
		i++;
	}
	while (echo_string[i] != NULL)
	{
		while (echo_string[i] == NULL)
			i++;
		write(1, echo_string[i], ft_strlen(echo_string[i]));
		i++;
		if (echo_string[i] != NULL)
			write(1, " ", 1);
	}
	if (new_line)
		write(1, "\n", 1);
	return (0);
}

int	check_builtin_type_and_run_bi(char ***envp, int *envp_size, t_cmd *bi)
{
	*envp = *envp;
	*envp_size = *envp_size;
	if (bi && bi->cmd && bi->cmd[0] != '\0')
	{
		if (ft_strncmp("echo", bi->cmd, 4) == 0)
			return (built_echo(bi->args));
	}
	else if (!(bi && bi->cmd))
		display_err_with_prefix("(null)", " command not found\n");
	else
		display_err_with_prefix(bi->cmd, " command not found\n");
	return (127);
}
