/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkolarov <dkolarov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 09:16:21 by dkolarov          #+#    #+#             */
/*   Updated: 2025/12/03 11:13:31 by dkolarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

/**
 * @brief Create a new pipe.
 *
 * Wraps the system call `pipe()` and returns 0 on success or -1 on error.
 * The file descriptors are written into `fds`.
 *
 * @param fds Two-element array that will receive the pipe file descriptors
 * @return int 0 on success, -1 on failure
 */
int	create_pipe(int fds[2])
{
	if (!fds)
		return (-1);
	if (pipe(fds) == -1)
	{
		perror("minishell: pipe");
		return (-1);
	}
	return (0);
}

/**
 * @brief Duplicate the read end of a pipe to STDIN for a child.
 *
 * Calls dup2(fds[0], STDIN_FILENO) and closes both pipe fds. Returns 0 on
 * success or -1 on error.
 *
 * @param fds Two-element pipe fd array
 * @return int 0 on success, -1 on error
 */
int	dup_pipe_in(int fds[2])
{
	if (dup2(fds[0], STDIN_FILENO) == -1)
	{
		perror("minishell: dup2");
		close(fds[0]);
		close(fds[1]);
		return (-1);
	}
	close(fds[0]);
	close(fds[1]);
	return (0);
}

/**
 * @brief Duplicate the write end of a pipe to STDOUT for a child.
 *
 * Calls dup2(fds[1], STDOUT_FILENO) and closes both pipe fds. Returns 0 on
 * success or -1 on error.
 *
 * @param fds Two-element pipe fd array
 * @return int 0 on success, -1 on error
 */
int	dup_pipe_out(int fds[2])
{
	if (dup2(fds[1], STDOUT_FILENO) == -1)
	{
		perror("minishell: dup2");
		close(fds[0]);
		close(fds[1]);
		return (-1);
	}
	close(fds[0]);
	close(fds[1]);
	return (0);
}

/**
 * @brief Close both ends of a pipe.
 *
 * Safely closes the provided pipe file descriptors. If `fds` is NULL the
 * function is a no-op.
 *
 * @param fds Two-element pipe fd array
 * @return void
 */
void	close_pipe(int fds[2])
{
	if (!fds)
		return ;
	close(fds[0]);
	close(fds[1]);
}
