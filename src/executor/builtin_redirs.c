/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_redirs.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjusta <mjusta@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 09:10:00 by dkolarov          #+#    #+#             */
/*   Updated: 2025/12/16 15:31:27 by mjusta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

/**
 * @brief Prepare redirections for a builtin executed in the parent.
 *
 * Opens any heredocs, saves the current STDIN/STDOUT file descriptors into
 * `sin`/`sout`, and applies the command's redirections. On failure it
 * restores/describes cleanup and returns -1.
 *
 * @param cmd Command that may contain redirections (and heredoc entries)
 * @param sh Shell state (used to access environment for heredoc expansion)
 * @param sin[out] Receives duplicated STDIN file descriptor on success
 * @param sout[out] Receives duplicated STDOUT file descriptor on success
 * @return int 0 on success, -1 on error
 */
int	prepare_builtin_redirs(t_cmd *cmd, t_shell *sh, int *sin, int *sout)
{
	(void)sh;
	*sin = dup(STDIN_FILENO);
	*sout = dup(STDOUT_FILENO);
	if (*sin == -1 || *sout == -1)
		return (perror("minishell: dup"), -1);
	if (apply_redirections(cmd->redirs) == -1)
	{
		dup2(*sin, STDIN_FILENO);
		dup2(*sout, STDOUT_FILENO);
		close(*sin);
		close(*sout);
		cleanup_heredocs(cmd->redirs);
		return (-1);
	}
	return (0);
}
