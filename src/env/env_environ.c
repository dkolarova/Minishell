/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_environ.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkolarov <dkolarov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 18:21:07 by mjusta            #+#    #+#             */
/*   Updated: 2025/11/19 10:54:01 by dkolarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

/**
 * @brief Count how many environment variables are marked as exported.
 */
static int	count_export(t_env *env)
{
	int	count;

	count = 0;
	while (env)
	{
		if (env->exported)
			count++;
		env = env->next;
	}
	return (count);
}

/**
 * @brief Join a key and value from the environment list into "KEY=VALUE".
 * 
 * @return Newly allocated string in "KEY=VALUE" format, or NULL on failure.
 */
static char	*join_key_value(t_env *env)
{
	char	*tmp;
	char	*result;

	tmp = ft_strjoin(env->key, "=");
	if (!tmp)
		return (NULL);
	result = ft_strjoin(tmp, env->value);
	free(tmp);
	return (result);
}

/**
 * @brief Convert internal env linked list into a char ** environment array.
 * 
 * Each string is in the format "KEY=VALUE" and only exported vars are included.
 * 
 * @return A NULL-terminated array of strings, or NULL on allocation failure.
 */
char	**env_to_environ(t_env *env)
{
	char	**environ;
	int		count;
	int		i;

	count = count_export(env);
	environ = malloc(sizeof(char *) * (count + 1));
	if (!environ)
		return (NULL);
	i = 0;
	while (env)
	{
		if (env->exported)
		{
			environ[i] = join_key_value(env);
			if (!environ[i])
				return (free_char_arr(environ), NULL);
			i++;
		}
		env = env->next;
	}
	environ[i] = 0;
	return (environ);
}
