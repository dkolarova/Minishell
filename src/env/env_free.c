/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_free.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjusta <mjusta@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 18:19:21 by mjusta            #+#    #+#             */
/*   Updated: 2025/11/21 15:09:41 by mjusta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

/**
 * @brief Prints enviromental list in console.
 */
void	print_env(t_env *env)
{
	while (env)
	{
		if (env->exported)
		{
			if (env->value)
				ft_printf("%s=%s\n", env->key, env->value);
			else
				ft_printf("%s=\n", env->key);
		}
		env = env->next;
	}
}
