/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_pwd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbocktor <jbocktor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 13:09:10 by jbocktor          #+#    #+#             */
/*   Updated: 2024/05/27 13:20:41 by jbocktor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	built_pwd(char **pwd)
{
	char	*path;
	
	(void)pwd;
	path = getcwd(NULL, 0);
	write(1, path, ft_strlen(path));
	write(1, "\n", 1);
	return (0);
}
