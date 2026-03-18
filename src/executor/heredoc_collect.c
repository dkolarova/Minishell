/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_collect.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjusta <mjusta@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 08:48:00 by dkolarov          #+#    #+#             */
/*   Updated: 2026/01/02 20:32:21 by mjusta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "get_line.h"

static int	heredoc_write_line(t_redir *r, int fd, char *line);
static int	heredoc_cleanup(t_redir *r, int fd, char *limiter);
static int	heredoc_process_line(t_redir *r, int fd,
				char *line, char *limiter);
static int	heredoc_read_once(t_redir *r, int fd,
				char *limiter, int *stop);

// Main heredoc loop: read lines until limiter and write to fd
int	heredoc_write_loop(t_redir *r, int fd, char *limiter)
{
	int	stop;
	int	ret;

	stop = 0;
	while (!stop)
	{
		if (g_sig == 130)
			return (heredoc_cleanup(r, fd, limiter));
		ret = heredoc_read_once(r, fd, limiter, &stop);
		if (ret == -1)
			return (-1);
	}
	free(limiter);
	close(fd);
	return (0);
}

// Write one line to heredoc file, appending a newline
static int	heredoc_write_line(t_redir *r, int fd, char *line)
{
	ssize_t	w1;
	ssize_t	w2;

	w1 = write(fd, line, ft_strlen(line));
	w2 = write(fd, "\n", 1);
	if (w1 < 0 || w2 < 0)
	{
		free(line);
		close(fd);
		if (r && r->arg)
			unlink(r->arg);
		return (-1);
	}
	return (0);
}

// Cleanup resources on error: free limiter, close fd, unlink file
static int	heredoc_cleanup(t_redir *r, int fd, char *limiter)
{
	free(limiter);
	close(fd);
	if (r && r->arg)
		unlink(r->arg);
	return (-1);
}

// Process a single line: strip newline, check limiter, or write
static int	heredoc_process_line(t_redir *r, int fd,
	char *line, char *limiter)
{
	size_t	len;

	len = ft_strlen(line);
	if (len > 0 && line[len - 1] == '\n')
		line[len - 1] = '\0';
	if (ft_strncmp(line, limiter, ft_strlen(limiter) + 1) == 0)
		return (free(line), 1);
	if (heredoc_write_line(r, fd, line) == -1)
		return (free(line), -1);
	free(line);
	return (0);
}

// Read one line from stdin and process it, set stop if done
static int	heredoc_read_once(t_redir *r, int fd,
	char *limiter, int *stop)
{
	char	*line;
	ssize_t	nread;
	size_t	cap;

	(void)write(1, "> ", 2);
	line = NULL;
	cap = 0;
	nread = my_getline(&line, &cap);
	if (nread == -1)
	{
		if (g_sig == 130)
			return (heredoc_cleanup(r, fd, limiter));
		free(line);
		*stop = 1;
		return (0);
	}
	nread = heredoc_process_line(r, fd, line, limiter);
	if (nread != 0)
		*stop = 1;
	return (nread);
}
