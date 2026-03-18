/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_get_set_helpers.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkolarov <dkolarov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 10:19:27 by dkolarov          #+#    #+#             */
/*   Updated: 2025/11/19 11:11:51 by dkolarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

/**
 * @brief Create a new environment node.
 *
 * @param key Key string to duplicate into the node.
 * @param value Value string to duplicate into the node.
 * @param exported Export flag for the node.
 * @return t_env* Newly allocated node, or NULL on allocation failure.
 */
/**
 * @brief Allocate an empty environment node.
 *
 * @return t_env* New node or NULL.
 */
static t_env	*alloc_node(void)
{
	t_env	*node;

	node = malloc(sizeof(*node));
	if (!node)
		return (NULL);
	node->key = NULL;
	node->value = NULL;
	node->exported = 0;
	node->next = NULL;
	return (node);
}

/**
 * @brief Create a new environment node with key/value.
 */
static t_env	*new_node(const char *key, const char *value, int exported)
{
	t_env	*node;

	node = alloc_node();
	if (!node)
		return (NULL);
	if (key)
	{
		node->key = ft_strdup(key);
		if (!node->key)
			return (free(node), NULL);
	}
	if (value)
	{
		node->value = ft_strdup(value);
		if (!node->value)
		{
			free(node->key);
			return (free(node), NULL);
		}
	}
	node->exported = exported;
	return (node);
}

/**
 * @brief Update an existing environment node's value and export flag.
 *
 * @param cur Target node to update.
 * @param value New value to assign (duplicated internally).
 * @param exported New export flag.
 * @return int 0 on success, 1 on allocation failure.
 */
int	update_node(t_env *cur, const char *value, int exported)
{
	if (!cur)
		return (1);
	if (cur->value)
		free(cur->value);
	cur->value = NULL;
	if (value)
	{
		cur->value = ft_strdup(value);
		if (!cur->value)
			return (1);
	}
	cur->exported = exported;
	return (0);
}

/**
 * @brief Append a new key/value/exported triple to the end of the env list.
 *
 * @param env Pointer to env list head.
 * @param key Key string for new node.
 * @param value Value string for new node.
 * @param exported Export flag.
 * @return int 0 on success, 1 on allocation failure.
 */
int	append_node(t_env **env, const char *key, const char *value, int exported)
{
	t_env	*new;
	t_env	*cur;

	new = new_node(key, value, exported);
	if (!new)
		return (1);
	if (!*env)
	{
		*env = new;
		return (0);
	}
	cur = *env;
	while (cur->next)
		cur = cur->next;
	cur->next = new;
	return (0);
}
