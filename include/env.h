/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkolarov <dkolarov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 17:55:09 by mjusta            #+#    #+#             */
/*   Updated: 2025/12/03 11:12:47 by dkolarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

# include "libft.h"
# include "minishell.h"
# include "cleanup_sh.h"
# include <unistd.h>

typedef struct s_env
{
	char			*key;
	char			*value;
	int				exported;
	struct s_env	*next;
}	t_env;

int		update_node(t_env *cur, const char *value, int exported);
int		append_node(t_env **env, const char *key, const char *val, int export);

/* env (src/utils/env_*.c) */

t_env	*env_init(char **envp);
char	*env_get(t_env *env, const char *key);
int		env_set(t_env **env, char *key, char *value, int exported);
int		env_unset(t_env **env, char *key);
char	**env_to_environ(t_env *env);

/* small convenience wrappers used by builtins */
int		set_env(t_env **env, const char *key, const char *maybe_value);
int		unset_env(t_env **env, const char *key);

/* printing/export helpers (src/builtins/export_print.c) */

void	print_env(t_env *env);
void	print_env_export(t_env *env);

#endif