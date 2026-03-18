/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_line.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjusta <mjusta@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 14:10:03 by diana             #+#    #+#             */
/*   Updated: 2026/01/02 21:26:40 by mjusta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_LINE_H
# define GET_LINE_H

# include <sys/types.h>
# include <unistd.h>
# include <stdlib.h>

typedef struct s_buffer
{
	char	*buf;
	size_t	len;
	size_t	cap;
}	t_buffer;

ssize_t	my_getline(char **lineptr, size_t *n);

#endif