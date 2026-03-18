/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_child.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjusta <mjusta@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 22:17:00 by mjusta            #+#    #+#             */
/*   Updated: 2025/12/27 02:48:41 by mjusta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signals.h"
#include <unistd.h>

// Restore default signals in child process
void	install_child_signal_handlers(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

// Set shell signal mode (parent, heredoc, exit, etc.)
void	set_signal(t_signal_mode mode, t_shell *msh)
{
	if (mode == STOP_RESTORE)
		install_parent_signal_handlers();
	else if (mode == STOP_QUIT)
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (mode == EXIT)
	{
		write(1, "exit\n", 5);
		if (msh)
			msh->running = 0;
	}
	else if (mode == HEREDOC || mode == HEREDOC_PAUSE)
	{
		g_sig = 0;
		install_heredoc_signal();
		g_sig = 0;
		rl_done = 0;
	}
}

// Print message for child terminated by a signal (bash style)
int	print_signal_status(int sig)
{
	if (sig == SIGINT)
		write(1, "\n", 1);
	else if (sig == SIGQUIT)
		write(1, "Quit (core dumped)\n", 19);
	return (128 + sig);
}
