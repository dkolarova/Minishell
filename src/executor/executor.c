/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjusta <mjusta@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 08:16:13 by dkolarov          #+#    #+#             */
/*   Updated: 2025/12/16 16:36:21 by mjusta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static int	exec_single_command(t_cmd *cmd, t_shell *sh);
static int	exec_builtin(t_builtin builtin_id, char **argv, t_shell *sh);
static int	handle_builtin_redirs(t_cmd *cmd, t_shell *sh, t_builtin id);

/**
 * @brief Main executor entry point.
 *
 * Dispatches either a single command or a pipeline depending on the AST.
 * Returns the exit status of the executed command(s).
 *
 * @param ast Top-level AST (pipeline of commands)
 * @param sh Shell state
 * @return int Exit status
 */
void	exec_ast(t_shell *sh)
{
	if (!sh->ast)
		return ;
	if (!sh->ast->next)
	{
		sh->last_status = exec_single_command(sh->ast, sh);
		return ;
	}
	sh->last_status = run_pipeline(sh);
}

/**
 * @brief Run a builtin in the parent with redirections applied.
 *
 * Prepares heredocs and applies redirections in the parent so builtins that
 * modify shell state run in the parent process. Restores original std fds
 * before returning.
 *
 * @param cmd Command node containing argv and redirections
 * @param sh Shell state
 * @param id Builtin identifier
 * @return int builtin return status, or 1 on redirection error
 */
static int	handle_builtin_redirs(t_cmd *cmd, t_shell *sh, t_builtin id)
{
	int	sin;
	int	sout;
	int	status;

	if (prepare_builtin_redirs(cmd, sh, &sin, &sout) == -1)
		return (1);
	status = exec_builtin(id, cmd->argv, sh);
	dup2(sin, STDIN_FILENO);
	dup2(sout, STDOUT_FILENO);
	close(sin);
	close(sout);
	cleanup_heredocs(cmd->redirs);
	return (status);
}

/**
 * @brief Execute a single command (builtin or external).
 *
 * Determines whether the command is a builtin and dispatches accordingly.
 * Builtins without redirections are executed directly; those with redirs are
 * handled in the parent so they can affect shell state.
 *
 * @param cmd Command node
 * @param sh Shell state
 * @return int Exit status of the command
 */
static int	exec_single_command(t_cmd *cmd, t_shell *sh)
{
	t_builtin	id;
	int			ret;

	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (0);
	if (cmd->redirs)
	{
		set_signal(HEREDOC, sh);
		ret = open_heredoc(cmd->redirs, sh->env);
		set_signal(STOP_RESTORE, sh);
		if (ret == -1)
			return (130);
	}
	id = builtin_from(cmd->argv[0]);
	if (id != B_NONE)
	{
		if (cmd->redirs)
			return (handle_builtin_redirs(cmd, sh, id));
		return (exec_builtin(id, cmd->argv, sh));
	}
	return (exec_external_command(cmd, sh));
}

/**
 * @brief Dispatch builtin execution by identifier.
 *
 * Calls the matching builtin implementation and returns its status. For
 * builtins that need environment or shell access the shell state is passed.
 *
 * @param id Builtin identifier
 * @param argv Arguments vector
 * @param sh Shell state
 * @return int builtin return status
 */
static int	exec_builtin(t_builtin id, char **argv, t_shell *sh)
{
	if (id == B_ECHO)
		return (bi_echo(argv));
	else if (id == B_CD)
		return (bi_cd(argv, &sh->env));
	else if (id == B_PWD)
		return (bi_pwd(argv));
	else if (id == B_EXPORT)
		return (bi_export(argv, &sh->env));
	else if (id == B_UNSET)
		return (bi_unset(argv, &sh->env));
	else if (id == B_ENV)
		return (bi_env(argv, sh->env));
	else if (id == B_EXIT)
		return (bi_exit(argv, sh));
	return (1);
}
