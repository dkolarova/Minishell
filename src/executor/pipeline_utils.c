/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkolarov <dkolarov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 19:11:11 by dkolarov          #+#    #+#             */
/*   Updated: 2025/12/03 11:13:26 by dkolarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

/**
 * @brief Prepare heredoc temporary files for a pipeline.
 *
 * Iterates the pipeline and opens heredoc temporary files for each command
 * that contains a heredoc redirection. On error it cleans up any created
 * heredocs and returns -1.
 *
 * @param ast Pipeline AST
 * @param env Environment used for expansion (currently unused)
 * @return int 0 on success, -1 on error
 */
int	prepare_heredocs(t_ast *ast, t_env *env)
{
	t_cmd	*cmd;

	cmd = ast;
	while (cmd)
	{
		if (cmd->redirs && open_heredoc(cmd->redirs, env) == -1)
		{
			cleanup_heredocs(ast->redirs);
			return (-1);
		}
		cmd = cmd->next;
	}
	return (0);
}

/**
 * @brief Wait for all child processes and return the last status.
 *
 * Calls waitpid(-1, ...) in a loop until no children remain. Returns the
 * last collected status word (usable with WIFEXITED/WEXITSTATUS).
 *
 * @return int last child status word
 */
int	wait_collect_status(void)
{
	int	status;

	status = 0;
	while (waitpid(-1, &status, 0) > 0)
		;
	return (status);
}
