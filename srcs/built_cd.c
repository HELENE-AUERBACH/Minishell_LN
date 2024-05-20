/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_cd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbocktor <jbocktor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 13:08:44 by jbocktor          #+#    #+#             */
/*   Updated: 2024/05/20 16:28:41 by jbocktor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int	built_cd(char ***envp, int *envp_size, char **cd)
{
	/*int		i;
	char	*value;
	char	*path;

	value = NULL;
	if (get_value_from_envp(&value, "HOME", *envp_size, *envp) != 0)
		return (3);
	i = 0;
	while (cd)
		i++;
	if (i > 2)
	{
		// "bash: %s: too many arguments\n";
		return (0);
	}
	
	chdir(path);*/
	(void)envp;
	(void)cd;
	(void)envp_size;
	return (0);
}
