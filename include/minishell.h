/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjusta <mjusta@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 22:19:20 by mjusta            #+#    #+#             */
/*   Updated: 2025/11/21 16:37:05 by mjusta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <sys/types.h>

typedef struct s_env	t_env;
typedef struct s_tok	t_tok;
typedef struct s_cmd	t_cmd;
typedef t_cmd			t_ast;

typedef struct s_shell
{
	t_env	*env;
	int		last_status;
	int		running;
	char	**path_dirs;
	pid_t	pid;
	t_tok	*tokens;
	t_ast	*ast;
}	t_shell;

#endif
