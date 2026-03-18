/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_wrappers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjusta <mjusta@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 10:45:28 by dkolarov          #+#    #+#             */
/*   Updated: 2025/11/23 04:05:12 by mjusta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

/**
 * @brief Parse assignment in the form "KEY=VALUE".
 *
 * Extracts key and value from a single string containing an '=' character.
 * Allocates both key_out and val_out on success; caller must free them.
 *
 * @param assignment The input string containing KEY=VALUE
 * @param key_out Output pointer that receives a newly-allocated key
 * @param val_out Output pointer that receives a newly-allocated value
 * @return int 0 on success, 1 on error (invalid input or allocation failure)
 */
static int	parse_assignment_no_value(const char *assignment,
			char **key_out, char **val_out)
{
	char	*eq;

	if (!assignment || !key_out || !val_out)
		return (1);
	eq = ft_strchr(assignment, '=');
	if (!eq)
		return (1);
	*key_out = ft_substr(assignment, 0, (size_t)(eq - assignment));
	if (!*key_out)
		return (1);
	*val_out = ft_strdup(eq + 1);
	if (!*val_out)
	{
		free(*key_out);
		return (1);
	}
	return (0);
}

/**
 * @brief Parse either a full assignment or a key with a separate value.
 *
 * If `assignment_or_key` contains an '=' the function behaves like
 * parse_assignment_no_value. Otherwise it duplicates the key and uses
 * `maybe_value` as the value. Allocates outputs on success; caller frees.
 *
 * @param assignment_or_key The input which may be "KEY=VALUE" or just "KEY"
 * @param maybe_value The value to use if assignment_or_key does not contain '='
 * @param key_out Output pointer that receives a newly-allocated key
 * @param val_out Output pointer that receives a newly-allocated value
 * @return int 0 on success, 1 on error (invalid input or allocation failure)
 */
static int	parse_assignment_with_value(const char *assignment_or_key,
			const char *maybe_value, char **key_out, char **val_out)
{
	char	*eq;

	if (!assignment_or_key || !maybe_value || !key_out || !val_out)
		return (1);
	eq = ft_strchr(assignment_or_key, '=');
	if (eq)
		return (parse_assignment_no_value(assignment_or_key,
				key_out, val_out));
	*key_out = ft_strdup(assignment_or_key);
	if (!*key_out)
		return (1);
	*val_out = ft_strdup(maybe_value);
	if (!*val_out)
	{
		free(*key_out);
		return (1);
	}
	return (0);
}

/**
 * @brief General wrapper that parses an assignment or key+value pair.
 *
 * Delegates to parse_assignment_no_value when maybe_value is NULL (treating
 * the first argument as a full assignment), otherwise calls
 * parse_assignment_with_value. Returns 0 on success and fills outputs.
 *
 * @param assignment_or_key Either "KEY=VALUE" or "KEY"
 * @param maybe_value If non-NULL, the value to associate with the key
 * @param key_out Output pointer that receives a newly-allocated key
 * @param val_out Output pointer that receives a newly-allocated value
 * @return int 0 on success, 1 on error
 */
static int	parse_assignment(const char *assignment_or_key,
			const char *maybe_value, char **key_out, char **val_out)
{
	if (!assignment_or_key || !key_out || !val_out)
		return (1);
	if (!maybe_value)
		return (parse_assignment_no_value(assignment_or_key,
				key_out, val_out));
	return (parse_assignment_with_value(assignment_or_key,
			maybe_value, key_out, val_out));
}

/**
 * @brief Set an environment variable (wrapper).
 *
 * Accepts a single string "KEY=VALUE" when `maybe_value` is NULL, or a
 * separate key and value otherwise. On success the variable is inserted or
 * updated in the environment list.
 *
 * @param env Pointer to environment list head pointer
 * @param key Either "KEY=VALUE" or just the key
 * @param maybe_value Optional value when key and value are passed separately
 * @return int 0 on success, 1 on error
 */
int	set_env(t_env **env, const char *key, const char *maybe_value)
{
	char	*key_dup;
	char	*val_dup;

	if (!key || !env)
		return (1);
	if (parse_assignment(key, maybe_value,
			&key_dup, &val_dup) != 0)
		return (1);
	if (env_set(env, key_dup, val_dup, 1) != 0)
	{
		free(key_dup);
		free(val_dup);
		return (1);
	}
	free(key_dup);
	free(val_dup);
	return (0);
}

/**
 * @brief Unset (remove) an environment variable.
 *
 * Removes the variable `key` from the environment list if present.
 *
 * @param env Pointer to environment list head pointer
 * @param key Name of the variable to remove
 * @return int 0 on success or if key not found, 1 on invalid input
 */
int	unset_env(t_env **env, const char *key)
{
	if (!env || !key)
		return (1);
	return (env_unset(env, (char *)key));
}
