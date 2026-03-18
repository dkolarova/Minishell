/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkolarov <dkolarov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 10:40:24 by mjusta            #+#    #+#             */
/*   Updated: 2025/12/03 11:12:41 by dkolarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

/**
 * @brief Copy one 'KEY=VALUE' entry from envp 
 *        into the internal env linked list.
 * 
 * @param env Pointer to environment list head.
 * @param entry One string from envp (expected format KEY=VALUE).
 * @return int 0 on success, 1 on allocation failure.
 */
static int	copy_entry_to_env(t_env **env, const char *entry)
{
	char	*equal_pos;
	char	*key_dup;
	char	*value_dup;
	int		set_check;

	equal_pos = ft_strchr(entry, '=');
	if (!equal_pos)
		return (0);
	key_dup = ft_substr(entry, 0, (size_t)(equal_pos - entry));
	if (!key_dup)
		return (1);
	value_dup = ft_strdup(equal_pos + 1);
	if (!value_dup)
		return (free(key_dup), 1);
	set_check = env_set(env, key_dup, value_dup, 1);
	free(key_dup);
	free(value_dup);
	if (set_check != 0)
		return (1);
	return (0);
}

/**
* @brief Normalize SHLVL: clamp weird values and increment by 1.
*
* @param env Pointer to environment list head.
*/
static void	normalize_shlvl(t_env **env)
{
	char	*shlvl_value;
	int		level;
	char	*level_str;

	shlvl_value = env_get(*env, "SHLVL");
	if (shlvl_value)
		level = ft_atoi(shlvl_value);
	else
		level = 0;
	if (level < 0 || level > 1000)
		level = 0;
	level_str = ft_itoa((int)(level + 1));
	if (!level_str)
		return ;
	if (env_set(env, "SHLVL", level_str, 1) != 0)
	{
		free(level_str);
		return ;
	}
	free(level_str);
}

/**
 * @brief Insert PWD into the environment if it's missing.
 *
 * This mirrors the original inline logic from env_init: if PWD is not set
 * and getcwd succeeds, duplicate the cwd and call env_set. env_set duplicates
 * internally, so this helper frees the temporary copy regardless of the
 * env_set result. Failure to set PWD is non-fatal (keeps previous behavior).
 */
static void	insert_pwd_if_missing(t_env **head)
{
	char	cwd_buf[4096];

	cwd_buf[0] = '\0';
	if (!env_get(*head, "PWD") && getcwd(cwd_buf, sizeof(cwd_buf)))
	{
		(void)env_set(head, "PWD", cwd_buf, 1);
	}
}

/**
 * @brief Initialize internal enviroment List from envp.
 *        Ensures sane SHLVL and inserts PWD if missing.
 * 
 * @param envp Parent environment.
 * @return t_env* Head of newly built environment list, or NULL on error.
 */
t_env	*env_init(char **envp)
{
	t_env	*head;
	int		i;

	head = NULL;
	i = 0;
	while (envp && envp[i])
	{
		if (copy_entry_to_env(&head, envp[i]))
			return (env_free(head), NULL);
		i++;
	}
	normalize_shlvl(&head);
	insert_pwd_if_missing(&head);
	return (head);
}
