/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjusta <mjusta@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 22:14:27 by mjusta            #+#    #+#             */
/*   Updated: 2025/12/30 00:47:56 by mjusta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

# define _POSIX_C_SOURCE 200809L
# include <signal.h>
# include "minishell.h"
# include <readline/readline.h>

typedef struct s_shell			t_shell;

/* global variable storing the last received signal */
extern volatile sig_atomic_t	g_sig;

/* signal handling modes used by set_signal() */
typedef enum e_signal_mode
{
	STOP_RESTORE,
	STOP_QUIT,
	EXIT,
	HEREDOC,
	HEREDOC_PAUSE
}	t_signal_mode;

/* parent / interactive shell signals */
void	reset_prompt(int sig);
void	install_parent_signal_handlers(void);
void	heredoc_sigint(int sig);
void	install_heredoc_signal(void);

/* child process signals */
void	install_child_signal_handlers(void);

/* utility functions */
void	set_signal(t_signal_mode mode, t_shell *msh);
int		print_signal_status(int sig);

#endif