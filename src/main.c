/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjusta <mjusta@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 23:21:06 by mjusta            #+#    #+#             */
/*   Updated: 2026/01/05 18:27:05 by mjusta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"
#include "expander.h"
#include "parser.h"
#include "executor.h"
#include "cleanup_sh.h"
#include "env.h"
#include "libft_output.h"
#include "signals.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>

static void	process_line(t_shell *shell, char *line)
{
	if (lex_line(line, &shell->tokens) != 0)
	{
		shell->last_status = 2;
		return ;
	}
	if (expand_tokens(shell) != 0)
	{
		shell->last_status = 2;
		return ;
	}
	if (parse_tokens(shell->tokens, &shell->ast) != 0)
	{
		shell->last_status = 2;
		return ;
	}
	exec_ast(shell);
}

static void	loop_cleanup(t_shell *shell)
{
	free_tokens(shell->tokens);
	ast_free(shell->ast);
	shell->tokens = NULL;
	shell->ast = NULL;
}

static void	shell_loop(t_shell *shell)
{
	char	*line;

	while (shell->running)
	{
		line = readline("minishell$ ");
		if (!line)
			break ;
		if (g_sig == 130)
		{
			shell->last_status = 130;
			g_sig = 0;
		}
		if (*line)
		{
			add_history(line);
			process_line(shell, line);
		}
		free(line);
		loop_cleanup(shell);
		g_sig = 0;
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argv;
	if (argc != 1)
	{
		ft_putstr_fd("Run './minishell' without parameters!\n", 2);
		return (1);
	}
	shell.env = env_init(envp);
	shell.last_status = 0;
	shell.running = 1;
	shell.path_dirs = NULL;
	shell.pid = 0;
	shell.tokens = NULL;
	shell.ast = NULL;
	rl_catch_signals = 0;
	install_parent_signal_handlers();
	shell_loop(&shell);
	cleanup_shell(&shell);
	rl_clear_history();
	return (shell.last_status);
}
