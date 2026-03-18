/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkolarov <dkolarov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 00:55:44 by mjusta            #+#    #+#             */
/*   Updated: 2025/12/08 13:07:45 by dkolarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include <unistd.h>
# include "libft.h"

# ifndef PATH_MAX_LEN 
#  define PATH_MAX_LEN 4096
# endif

typedef struct s_env	t_env;
typedef struct s_shell	t_shell;

/* builtin id */
typedef enum e_builtin
{
	B_NONE = 0,
	B_ECHO,
	B_CD,
	B_PWD,
	B_EXPORT,
	B_UNSET,
	B_ENV,
	B_EXIT
}	t_builtin;

/* detect builtin from argv[0] */

t_builtin	builtin_from(const char *cmd);

/* cd/export/unset/exit */

int			is_stateful_builtin(t_builtin id);

/* run in current process (no fork) when allowed */

int			bi_echo(char **argv);
int			bi_cd(char **argv, t_env **env);
int			bi_pwd(char **argv);
int			bi_export(char **argv, t_env **env);
int			bi_unset(char **argv, t_env **env);
int			bi_env(char **argv, t_env *env);
int			bi_exit(char **argv, t_shell *sh);

/* export */
int			bi_export(char **argv, t_env **env);
void		export_process_arg(char *arg, t_env **env);
void		export_set_or_mark(char *arg, t_env **env);
int			is_valid_identifier(const char *s);

#endif
