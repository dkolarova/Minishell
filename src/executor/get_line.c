/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_line.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjusta <mjusta@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 13:04:28 by diana             #+#    #+#             */
/*   Updated: 2026/01/02 21:27:18 by mjusta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "get_line.h"

static ssize_t	fail_getline(char **lineptr, t_buffer *b);
static int		read_loop(t_buffer *b);
static int		read_char(char *c);
static int		grow_buffer(t_buffer *b, size_t need);

/*
 * Minimal getline replacement that reads from STDIN (file descriptor 0).
 * Reads until '\n' or EOF.
 * Returns number of bytes read (including '\n' if present),
 * or -1 on EOF/error.
 * On -1, *lineptr is set to NULL.
 */
ssize_t	my_getline(char **lineptr, size_t *n)
{
	t_buffer	buffer;
	int			r;

	(void)n;
	buffer.buf = NULL;
	buffer.len = 0;
	buffer.cap = 0;
	r = read_loop(&buffer);
	if (r <= 0 && buffer.len == 0)
		return (fail_getline(lineptr, &buffer));
	if (!grow_buffer(&buffer, buffer.len + 1))
		return (fail_getline(lineptr, &buffer));
	buffer.buf[buffer.len] = '\0';
	*lineptr = buffer.buf;
	return ((ssize_t)buffer.len);
}

static ssize_t	fail_getline(char **lineptr, t_buffer *b)
{
	if (b->buf)
		free(b->buf);
	*lineptr = NULL;
	return (-1);
}

static int	grow_buffer(t_buffer *b, size_t need)
{
	char	*tmp;
	size_t	newcap;

	if (b->cap >= need)
		return (1);
	newcap = b->cap;
	if (newcap == 0)
		newcap = 128;
	while (newcap < need)
		newcap *= 2;
	tmp = (char *)malloc(newcap);
	if (tmp == NULL)
		return (0);
	if (b->buf != NULL)
	{
		ft_memcpy(tmp, b->buf, b->len);
		free(b->buf);
	}
	b->buf = tmp;
	b->cap = newcap;
	return (1);
}

static int	read_char(char *c)
{
	ssize_t	r;

	r = read(STDIN_FILENO, c, 1);
	if (r <= 0)
		return ((int)r);
	return (1);
}

static int	read_loop(t_buffer *b)
{
	char	c;
	int		r;

	while (1)
	{
		r = read_char(&c);
		if (r != 1)
			return (r);
		if (!grow_buffer(b, b->len + 1))
			return (-1);
		b->buf[b->len] = c;
		b->len++;
		if (c == '\n')
			return (1);
	}
}
