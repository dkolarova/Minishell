/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_print.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkolarov <dkolarov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 11:19:50 by dkolarov          #+#    #+#             */
/*   Updated: 2025/11/07 10:28:25 by dkolarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "minishell.h"

/**
 * @brief Print exported environment variables for the `export` builtin.
 *
 * When `export` is invoked without arguments, this prints all variables
 * marked for export in a form similar to bash's `declare -x` output:
 * - `declare -x KEY="VALUE"` when a value is present
 * - `declare -x KEY` when no value is set
 *
 * @param env Pointer to the environment linked list
 * @return void
 */
void	print_env_export(t_env *env)
{
	while (env)
	{
		if (env->exported)
		{
			if (env->value)
				ft_printf("declare -x %s=\"%s\"\n", env->key, env->value);
			else
				ft_printf("declare -x %s\n", env->key);
		}
		env = env->next;
	}
}
