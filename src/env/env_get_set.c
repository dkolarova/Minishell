/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_get_set.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkolarov <dkolarov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 11:20:03 by mjusta            #+#    #+#             */
/*   Updated: 2025/11/19 10:59:45 by dkolarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

/**
 * @brief Find node with matching key; returns pointer to node or NULL.
 */
static t_env	*find_node(t_env *env, const char *key)
{
	size_t	len_key;

	if (!key || !*key)
		return (NULL);
	len_key = ft_strlen(key);
	while (env)
	{
		if (env->key && ft_strlen(env->key) == len_key
			&& ft_strncmp(env->key, key, len_key + 1) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

/**
 * @brief Get value by key. 
 * 
 *        DON'T FREE THE POINTER!
 * @return Pointer to internal value, or NULL.
 */
char	*env_get(t_env *env, const char *key)
{
	t_env	*hit;

	if (!key || !*key)
		return (NULL);
	hit = find_node(env, key);
	if (!hit)
		return (NULL);
	return (hit->value);
}

/**
 * @brief Set key to value (create or update).
 *        export flag is stored as-is.
 * @return 0 on succes, 1 on allocation failure or invalid key.
 */
int	env_set(t_env **env, char *key, char *value, int exported)
{
	t_env	*cur;

	if (!env || !key || !*key)
		return (1);
	cur = find_node(*env, key);
	if (cur)
		return (update_node(cur, value, exported));
	return (append_node(env, key, value, exported));
}

/**
 * @brief Remove an environment variable from the list by key.
 * 
 * @return 0 on success or if key not found, 1 on invalid input.
 */
int	env_unset(t_env **env, char *key)
{
	t_env	*cur;
	t_env	*prev;

	if (!env || !*env || !key || !*key)
		return (1);
	cur = *env;
	prev = NULL;
	while (cur)
	{
		if (cur == find_node(cur, key))
		{
			if (prev)
				prev->next = cur->next;
			else
				*env = cur->next;
			free(cur->key);
			free(cur->value);
			free(cur);
			return (0);
		}
		prev = cur;
		cur = cur->next;
	}
	return (0);
}
