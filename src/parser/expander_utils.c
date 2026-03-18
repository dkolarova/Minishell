/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjusta <mjusta@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 23:20:28 by mjusta            #+#    #+#             */
/*   Updated: 2025/11/17 23:46:42 by mjusta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expander.h"
#include "libft_string.h"

/**
 * @brief  Append a string to a dynamically managed destination.
 * 
 * @return 0 on success, 1 on allocation failure.
 */
int	append_string(char **dst, const char *src)
{
	char	*new;

	if (!src)
		return (0);
	if (*dst == NULL)
		new = ft_strdup(src);
	else
		new = ft_strjoin(*dst, src);
	if (!new)
		return (1);
	free(*dst);
	*dst = new;
	return (0);
}

/**
 * @brief  Append a single character to a dynamically managed string.
 * 
 * @return 0 on success, 1 on allocation failure.
 */
int	append_char(char **dst, char c)
{
	char	buffer[2];

	buffer[0] = c;
	buffer[1] = '\0';
	return (append_string(dst, buffer));
}
