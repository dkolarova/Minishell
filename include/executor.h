/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjusta <mjusta@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 00:56:42 by mjusta            #+#    #+#             */
/*   Updated: 2025/12/27 02:40:46 by mjusta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>
# include "builtins.h"
# include "env.h"
# include "minishell.h"
# include <errno.h>
# include "libft.h"
# include <unistd.h>
# include <sys/stat.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "signal.h"
# include "parser.h"
# include "signals.h"
# include <readline/readline.h>

typedef struct s_env	t_env;
typedef struct s_shell	t_shell;

// REDIRECTIONS

typedef enum e_redir_type
{
	R_IN = 1,
	R_OUT,
	R_APPEND,
	R_HEREDOC
}	t_redir_type;

typedef struct s_redir
{
	t_redir_type	type;
	char			*arg;
	int				quoted;
	struct s_redir	*pipe_next;
}	t_redir;

// COMMAND STRUCT

typedef struct s_cmd
{
	char			**argv;
	t_redir			*redirs;
	struct s_cmd	*next;
}	t_cmd;

/* top-level AST is just a pipeline list of t_cmd */
typedef t_cmd			t_ast;

// EXECUTOR CORE

/* execute parsed AST; returns exit status */
void	exec_ast(t_shell *sh);
int		exec_external_command(t_cmd *cmd, t_shell *shell);

/* helpers that may be used by executor */
/* Resolve a command path using the provided environment list (may be NULL).
 * If 'file' contains a '/', it is returned if accessible. Otherwise the PATH
 * value is read from 'env' and searched. Caller must free returned string.
 */
char	*resolve_command_path(char *file, t_env *env);
int		apply_redirections(t_redir *r);
int		open_heredoc(t_redir *r, t_env *env);
void	cleanup_heredocs(t_redir *r);
int		create_heredoc_temp(t_redir *r, int *fd_out, char **limiter_out);

/* helper used by executor when running builtins in parent with redirections */
int		prepare_builtin_redirs(t_cmd *cmd, t_shell *sh, int *sin, int *sout);

/* heredoc helper: collect lines from stdin and write to fd until limiter */
int		heredoc_write_loop(t_redir *r, int fd, char *limiter);

// PIPE HELPERS

int		create_pipe(int fds[2]);
int		dup_pipe_in(int fds[2]);
int		dup_pipe_out(int fds[2]);
void	close_pipe(int fds[2]);

// PIPELINE

/* run a full command pipeline; returns exit status of last command */
int		run_pipeline(t_shell *sh);

/* pipeline helper functions */
int		prepare_heredocs(t_ast *ast, t_env *env);
int		wait_collect_status(void);
void	child_exec(t_cmd *cmd, int prev_fd, int cur_pipe[2], t_shell *sh);

/* shared helper: prepare envp and resolve command path (path may be NULL) */
void	prepare_env_and_path(t_cmd *cmd, t_shell *sh,
			char ***envp_out, char **path_out);

#endif
