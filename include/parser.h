/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjusta <mjusta@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 00:57:02 by mjusta            #+#    #+#             */
/*   Updated: 2025/11/21 16:55:43 by mjusta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include <stddef.h>

typedef struct s_env	t_env;
typedef struct s_cmd	t_cmd;
typedef t_cmd			t_ast;

typedef struct s_tok	t_tok;
typedef struct s_shell	t_shell;

int		parse_tokens(t_tok *tokens, t_ast **ast);
int		parse_one_redir(t_tok **tokens, t_cmd *cmd);

void	ast_free(t_ast *ast);
size_t	argcount(char **argv);
int		is_redir_tok(t_tok *t);
int		check_pipe_syntax(t_tok *token);
void	free_cmd_resources(t_cmd *cmd);

#endif