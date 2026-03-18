/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkolarov <dkolarov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 10:38:11 by dkolarov          #+#    #+#             */
/*   Updated: 2025/12/08 10:49:07 by dkolarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtins.h"
#include "env.h"

/**
 * @brief Implement env builtin - print environment variables
 * 
 * @param argv Command arguments (not used for now)
 * @param env Environment variables
 * @return int Exit status
 */
int	bi_env(char **argv, t_env *env)
{
	if (argv[1])
	{
		ft_putstr_fd("minishell: env: options not supported\n", 2);
		return (1);
	}
	if (!env)
		return (0);
	print_env(env);
	return (0);
}
