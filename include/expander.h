/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjusta <mjusta@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 15:22:07 by mjusta            #+#    #+#             */
/*   Updated: 2025/11/21 17:02:46 by mjusta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANDER_H
# define EXPANDER_H

typedef struct s_shell	t_shell;

typedef struct s_quote
{
	int	singleq;
	int	doubleq;
}	t_quote;

int	expand_tokens(t_shell *shell);
int	append_string(char **dst, const char *src);
int	append_char(char **dst, char c);

#endif