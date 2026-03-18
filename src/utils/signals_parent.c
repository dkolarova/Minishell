/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_parent.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjusta <mjusta@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 18:27:35 by diana             #+#    #+#             */
/*   Updated: 2026/01/04 18:08:35 by mjusta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signals.h"
#include "minishell.h"
#include <unistd.h>
#include <readline/readline.h>
#include <signal.h>

volatile sig_atomic_t	g_sig = 0;

// SIGINT handler for heredoc input
void	heredoc_sigint(int sig)
{
	(void)sig;
	g_sig = 130;
	write(1, "\n", 1);
}

// Install heredoc-specific signals
void	install_heredoc_signal(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_handler = heredoc_sigint;
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

// SIGINT handler for interactive parent shell
void	reset_prompt(int sig)
{
	(void)sig;
	g_sig = 130;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

// Install signals for interactive parent shell
void	install_parent_signal_handlers(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_handler = reset_prompt;
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}
