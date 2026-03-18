/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_sh.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjusta <mjusta@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 15:49:28 by mjusta            #+#    #+#             */
/*   Updated: 2025/11/21 16:38:13 by mjusta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLEANUP_SH_H
# define CLEANUP_SH_H

typedef struct s_env	t_env;
typedef struct s_tok	t_tok;
typedef struct s_shell	t_shell;

void	cleanup_shell(t_shell *sh);
void	free_tokens(t_tok *token);
void	env_free(t_env *env);

#endif