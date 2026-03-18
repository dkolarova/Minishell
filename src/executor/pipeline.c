/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjusta <mjusta@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 19:11:28 by dkolarov          #+#    #+#             */
/*   Updated: 2026/01/02 20:20:05 by mjusta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static int	execute_pipeline_loop(t_ast *ast, t_shell *sh);
static int	pipeline_handle_cmd(t_cmd **pcmd, int *prev_fd, t_shell *sh);

/**
* @brief Execute a pipeline represented by \p ast.
 *
 * This function prepares heredocs, spawns children for each pipeline
 * segment, waits for the pipeline to finish and returns the final
 * exit status (mapped to shell conventions). On error it returns 1.
 *
 * @param ast Command list forming the pipeline (first element).
 * @param sh Shell runtime state used for environment and last_status.
 * @return int final exit status (0-255) or 1 on internal error.
 */
int	run_pipeline(t_shell *sh)
{
	int	status;
	int	ret;

	if (!sh->ast)
		return (sh->last_status);
	set_signal(HEREDOC, NULL);
	ret = prepare_heredocs(sh->ast, sh->env);
	set_signal(STOP_RESTORE, sh);
	if (ret == -1)
		return (cleanup_heredocs(sh->ast->redirs), 130);
	if (execute_pipeline_loop(sh->ast, sh) == 1)
		return (cleanup_heredocs(sh->ast->redirs), 1);
	set_signal(STOP_QUIT, sh);
	status = wait_collect_status();
	set_signal(STOP_RESTORE, sh);
	cleanup_heredocs(sh->ast->redirs);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

/**
 * @brief Iterate over pipeline commands and spawn handlers for each.
 *
 * Walks the linked list of commands starting at \p ast and calls
 * `pipeline_handle_cmd` for every node. The function maintains the
 * previous pipe read-end in \p prev_fd so consecutive commands are
 * connected correctly.
 *
 * @param ast First command node of the pipeline
 * @param sh Shell runtime context used during child execution
 * @return int 0 on success, 1 on error
 */
static int	execute_pipeline_loop(t_ast *ast, t_shell *sh)
{
	t_cmd	*cmd;
	int		prev_fd;

	prev_fd = -1;
	cmd = ast;
	while (cmd)
	{
		if (pipeline_handle_cmd(&cmd, &prev_fd, sh) == 1)
			return (1);
	}
	return (0);
}

/**
 * @brief Spawn a child for the current command and manage pipes.
 *
 * If the current command is not the last, a new pipe is created and the
 * child will write into its write end. The parent updates \p prev_fd to
 * the read-end for the next command. On error the function prints a
 * perror message and returns 1.
 *
 * @param pcmd Pointer to the current command pointer; advanced on success
 * @param prev_fd Pointer to the previous pipe read-end (-1 if none)
 * @param sh Shell runtime context passed to the child executor
 * @return int 0 on success, 1 on failure
 */
static int	pipeline_handle_cmd(t_cmd **pcmd, int *prev_fd, t_shell *sh)
{
	t_cmd	*cmd;
	int		cur_pipe[2];
	pid_t	pid;

	cmd = *pcmd;
	if (cmd->next && create_pipe(cur_pipe) == -1)
		return (perror("minishell: pipe"), 1);
	pid = fork();
	if (pid == -1)
		return (perror("minishell: fork"), 1);
	if (pid == 0)
		child_exec(cmd, *prev_fd, cur_pipe, sh);
	if (*prev_fd != -1)
		close(*prev_fd);
	if (cmd->next)
	{
		close(cur_pipe[1]);
		*prev_fd = cur_pipe[0];
	}
	*pcmd = cmd->next;
	return (0);
}
