/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redirs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjusta <mjusta@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 23:29:33 by mjusta            #+#    #+#             */
/*   Updated: 2025/11/21 16:54:49 by mjusta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "libft_string.h"
#include "executor.h"
#include "tokenizer.h"
#include <stdlib.h>

/**
 * @brief Check if a token represents a redirection operator.
 */
int	is_redir_tok(t_tok *t)
{
	return (t && (t->type == T_REDIR_IN || t->type == T_REDIR_OUT
			|| t->type == T_APPEND || t->type == T_HEREDOC));
}

/**
 * @brief Map lexer token types to internal redirection types.
 */
static t_redir_type	map_redir(t_token_type toktyp)
{
	if (toktyp == T_REDIR_IN)
		return (R_IN);
	if (toktyp == T_REDIR_OUT)
		return (R_OUT);
	if (toktyp == T_APPEND)
		return (R_APPEND);
	return (R_HEREDOC);
}

/**
 * Append a redirection node to a command's redirection list.
 */
static void	redir_push(t_redir **lst, t_redir *new)
{
	t_redir	*current;

	if (!*lst)
		return ((void)(*lst = new));
	current = *lst;
	while (current->pipe_next)
		current = current->pipe_next;
	current->pipe_next = new;
}

/**
 * @brief Allocate and initialize a new redirection node.
 * 
 * @return Pointer to new redirection node, or NULL on allocation failure.
 */
static t_redir	*init_redir(t_redir_type type, const char *arg, int quoted)
{
	t_redir	*redir;

	redir = (t_redir *)malloc(sizeof(*redir));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->arg = ft_strdup(arg);
	if (!redir->arg)
		return (free(redir), (t_redir *) NULL);
	redir->quoted = quoted;
	redir->pipe_next = NULL;
	return (redir);
}

/**
 * @brief Parse a single redirection from the token stream and attach it 
 * to a command.
 * 
 * @return 0 on success, -1 on error.
 */
int	parse_one_redir(t_tok **tokens, t_cmd *cmd)
{
	t_token_type	type;
	int				quoted;
	t_redir			*redir;

	if (!is_redir_tok(*tokens))
		return (-1);
	type = (*tokens)->type;
	*tokens = (*tokens)->next;
	if (!*tokens || ((*tokens)->type != T_STRING))
		return (-1);
	quoted = (type == T_HEREDOC && (*tokens)->type == T_STRING);
	redir = init_redir(map_redir(type), (*tokens)->value, quoted);
	if (!redir)
		return (-1);
	redir_push(&cmd->redirs, redir);
	*tokens = (*tokens)->next;
	return (0);
}
