/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_pwd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbocktor <jbocktor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 13:09:10 by jbocktor          #+#    #+#             */
/*   Updated: 2024/05/29 13:42:20 by jbocktor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	built_pwd(char **pwd)
{
	char	*path;

	path = getcwd(NULL, 0);
	if (!path)
	{
		display_err_with_prefix(pwd[0], " getcwd is NULL\n");
		return (-1);
	}
	write(1, path, ft_strlen(path));
	write(1, "\n", 1);
	free(path);
	return (0);
}
