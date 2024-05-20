/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_bi_minishell.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbocktor <jbocktor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 13:09:29 by jbocktor          #+#    #+#             */
/*   Updated: 2024/05/20 18:11:02 by hauerbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_builtin_type_and_run_bi(char ***envp, int *envp_size, t_cmd *bi,
		int fd2)
{
	*envp = *envp;
	*envp_size = *envp_size;
	if (bi && bi->cmd && bi->cmd[0] != '\0')
	{
		if (ft_strncmp("echo", bi->cmd, 4) == 0)
			return (built_echo(bi->args, fd2));
		if (ft_strncmp("cd", bi->cmd, 2) == 0)
			return (built_cd(envp, envp_size, bi->args));
		if (ft_strncmp("pwd", bi->cmd, 3) == 0)
			return (built_pwd(bi->args));
		if (ft_strncmp("export", bi->cmd, 6) == 0)
			return (built_export(envp, envp_size, bi->args, fd2));
		if (ft_strncmp("unset", bi->cmd, 5) == 0)
			return (built_unset(envp, envp_size, bi->args));
		if (ft_strncmp("env", bi->cmd, 3) == 0)
			return (built_env(*envp, bi->args, fd2));
		if (ft_strncmp("exit", bi->cmd, 4) == 0)
			return (built_exit(bi->args));
	}
	else if (!(bi && bi->cmd))
		display_err_with_prefix("(null)", " command not found\n");
	else
		display_err_with_prefix(bi->cmd, " command not found\n");
	return (127);
}
