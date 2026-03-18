/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkolarov <dkolarov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 09:12:04 by dkolarov          #+#    #+#             */
/*   Updated: 2025/12/03 11:13:19 by dkolarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static int	try_open_unique_heredoc(char **path_out, int *fd_out);
static char	*generate_heredoc_path(int index);

/**
 * @brief Create a temporary heredoc file and set its limiter.
 *
 * Calls try_open_unique_heredoc() to generate the file. Returns 0 on success
 * and sets *fd_out and *limiter_out. Returns -1 on failure.
 */
int	create_heredoc_temp(t_redir *r, int *fd_out, char **limiter_out)
{
	char	*path;
	int		fd;

	if (try_open_unique_heredoc(&path, &fd) == -1)
		return (-1);
	*limiter_out = ft_strdup(r->arg);
	if (!*limiter_out)
	{
		close(fd);
		free(path);
		return (-1);
	}
	free(r->arg);
	r->arg = path;
	*fd_out = fd;
	return (0);
}

static char	*generate_heredoc_path(int index)
{
	char	*num;
	char	*path;

	num = ft_itoa(index);
	if (!num)
		return (NULL);
	path = ft_strjoin("/tmp/minishell_heredoc_", num);
	free(num);
	return (path);
}

/**
 * @brief Attempt to create a unique temporary file for heredoc.
 *
 * Iterates indices until a unique file is created. Returns 0 on success
 * and sets *path_out and *fd_out. Returns -1 on failure.
 */
static int	try_open_unique_heredoc(char **path_out, int *fd_out)
{
	char	*path;
	int		fd;
	int		i;

	i = 0;
	fd = -1;
	while (i < 10000)
	{
		path = generate_heredoc_path(i);
		if (!path)
			return (-1);
		fd = open(path, O_CREAT | O_EXCL | O_RDWR, 0600);
		if (fd != -1)
		{
			*path_out = path;
			*fd_out = fd;
			return (0);
		}
		free(path);
		i++;
	}
	return (perror("minishell: tmpfile"), -1);
}
