/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjusta <mjusta@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 10:39:27 by dkolarov          #+#    #+#             */
/*   Updated: 2025/11/21 16:31:20 by mjusta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtins.h"
#include "env.h"

/**
 * @brief Implement unset builtin
 * 
 * @param argv Command arguments
 * @param env Environment variables
 * @return int Exit status
 */
int	bi_unset(char **argv, t_env **env)
{
	int	i;

	i = 1;
	if (!argv[1])
		return (0);
	while (argv[i])
	{
		unset_env(env, argv[i]);
		i++;
	}
	return (0);
}
