/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkolarov <dkolarov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 19:24:50 by dkolarov          #+#    #+#             */
/*   Updated: 2025/12/03 11:05:50 by dkolarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static void	exec_builtin_child(t_builtin id, t_cmd *cmd, t_shell *sh);
static void	exec_external_child(t_cmd *cmd, t_shell *sh);
static void	handle_execve_error(char *path, char **envp);

/**
 * @brief Child entrypoint for executing a single command in a pipeline.
 *
 * Sets up stdin/stdout according to the previous pipe and the current pipe,
 * applies redirections, installs child signal handlers and dispatches the
 * command to either a builtin runner or an external exec path.
 *
 * @param cmd Command node to execute
 * @param prev_fd Read-end fd from previous pipe (-1 if none)
 * @param cur_pipe Two-element array holding the pipe fds for this command
 * @param sh Shell state (environment, etc.)
 */
void	child_exec(t_cmd *cmd, int prev_fd, int cur_pipe[2], t_shell *sh)
{
	t_builtin	id;

	if (prev_fd != -1)
		dup2(prev_fd, STDIN_FILENO);
	if (cmd->next)
		dup2(cur_pipe[1], STDOUT_FILENO);
	if (prev_fd != -1)
		close(prev_fd);
	if (cmd->next)
	{
		close(cur_pipe[0]);
		close(cur_pipe[1]);
	}
	install_child_signal_handlers();
	if (cmd->redirs && apply_redirections(cmd->redirs) == -1)
		exit(1);
	if (!cmd->argv || !cmd->argv[0])
	{
		cleanup_shell(sh);
		exit(0);
	}
	id = builtin_from(cmd->argv[0]);
	if (id != B_NONE)
		exec_builtin_child(id, cmd, sh);
	exec_external_child(cmd, sh);
}

/**
 * @brief Execute a builtin in a child process and exit with its return code.
 *
 * Calls the corresponding builtin implementation and exits the process with
 * the builtin's returned status. The function never returns.
 *
 * @param id Builtin identifier
 * @param cmd Command node containing argv for the builtin
 * @param sh Shell state pointer for builtins that modify env/state
 */
static void	exec_builtin_child(t_builtin id, t_cmd *cmd, t_shell *sh)
{
	int	status;

	if (id == B_ECHO)
		status = bi_echo(cmd->argv);
	else if (id == B_CD)
		status = bi_cd(cmd->argv, &sh->env);
	else if (id == B_PWD)
		status = bi_pwd(cmd->argv);
	else if (id == B_EXPORT)
		status = bi_export(cmd->argv, &sh->env);
	else if (id == B_UNSET)
		status = bi_unset(cmd->argv, &sh->env);
	else if (id == B_ENV)
		status = bi_env(cmd->argv, sh->env);
	else if (id == B_EXIT)
		status = bi_exit(cmd->argv, sh);
	else
		status = 1;
	cleanup_shell(sh);
	exit(status);
}

/* prepare_env_and_path is provided in a shared source file */

/*
 * Handle execve failure: print perror, free resources and exit with
 * a shell-appropriate status (127 for ENOENT, 126 otherwise).
 */
static void	handle_execve_error(char *path, char **envp)
{
	perror("minishell");
	if (path)
		free(path);
	if (envp)
		free_char_arr(envp);
	if (errno == ENOENT)
		exit(127);
	exit(126);
}

/**
 * @brief Execute an external command in the child process.
 *
 * Builds the environ array, resolves the command path and calls execve.
 * On failure this function prints an error and exits with the appropriate
 * status code.
 *
 * @param cmd Command node containing argv
 * @param sh Shell state for environment lookup
 */
static void	exec_external_child(t_cmd *cmd, t_shell *sh)
{
	char	**envp;
	char	*path;

	prepare_env_and_path(cmd, sh, &envp, &path);
	if (!path)
	{
		if (ft_strchr(cmd->argv[0], '/'))
			ft_putstr_fd("minishell: file not found: ", 2);
		else
			ft_putstr_fd("minishell: command not found: ", 2);
		ft_putendl_fd(cmd->argv[0], 2);
		free_char_arr(envp);
		cleanup_shell(sh);
		exit(127);
	}
	execve(path, cmd->argv, envp);
	handle_execve_error(path, envp);
}
