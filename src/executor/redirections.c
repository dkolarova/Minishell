/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diana <diana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 10:55:55 by dkolarov          #+#    #+#             */
/*   Updated: 2025/12/23 12:21:32 by diana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static int	apply_single_redir(t_redir *r);

/**
 * @brief Apply a list of redirections to the current process.
 *
 * Iterates the redirection list and opens the target files or heredoc files,
 * then dup2()s them to STDIN or STDOUT as appropriate. Returns 0 on success
 * or -1 on error.
 */
int	apply_redirections(t_redir *r)
{
	while (r)
	{
		if (apply_single_redir(r) == -1)
			return (-1);
		r = r->pipe_next;
	}
	return (0);
}

/* Apply a single redirection: open target and dup2 to stdin/stdout. */
static int	apply_single_redir(t_redir *r)
{
	int	fd;
	int	target;

	if (r->type == R_IN || r->type == R_HEREDOC)
		fd = open(r->arg, O_RDONLY);
	else if (r->type == R_OUT)
		fd = open(r->arg, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (r->type == R_APPEND)
		fd = open(r->arg, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = -1;
	if (fd == -1)
		return (perror("minishell: open"), -1);
	if (r->type == R_IN || r->type == R_HEREDOC)
		target = STDIN_FILENO;
	else
		target = STDOUT_FILENO;
	if (dup2(fd, target) == -1)
	{
		close(fd);
		return (perror("minishell: dup2"), -1);
	}
	close(fd);
	return (0);
}

/**
 * @brief Open heredocs for a list of redirections.
 *
 * Calls helper functions in heredoc_utils.c to create temp files
 * and write heredoc contents.
 */
int	open_heredoc(t_redir *r, t_env *env)
{
	int		fd;
	char	*limiter;

	(void)env;
	while (r)
	{
		if (r->type == R_HEREDOC)
		{
			if (create_heredoc_temp(r, &fd, &limiter) == -1)
				return (-1);
			if (heredoc_write_loop(r, fd, limiter) == -1)
				return (-1);
		}
		r = r->pipe_next;
	}
	return (0);
}

/**
 * @brief Remove heredoc temporary files and clear r->arg.
 */
void	cleanup_heredocs(t_redir *r)
{
	while (r)
	{
		if (r->type == R_HEREDOC && r->arg)
		{
			unlink(r->arg);
			free(r->arg);
			r->arg = NULL;
		}
		r = r->pipe_next;
	}
}
