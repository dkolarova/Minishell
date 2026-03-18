/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_external.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjusta <mjusta@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 08:07:45 by dkolarov          #+#    #+#             */
/*   Updated: 2025/12/16 15:30:57 by mjusta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static void	handle_redirections(t_cmd *cmd, t_shell *shell);
static void	exec_child_process(t_cmd *cmd, t_shell *shell);

/**
 * @brief Execute an external command (fork manager).
 *
 * Forks a child process to run the external command. The parent waits for
 * the child and returns the child's exit status (or a signal-derived code).
 * The child calls exec_child_process() to perform the exec.
 *
 * @param cmd Command node to execute
 * @param env Pointer to environment list head pointer
 * @return int exit status of the child, or 1 on fork error
 */
int	exec_external_command(t_cmd *cmd, t_shell *shell)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		return (ft_putstr_fd("minishell: fork failed\n", 2), 1);
	if (pid == 0)
		exec_child_process(cmd, shell);
	set_signal(STOP_QUIT, shell);
	waitpid(pid, &status, 0);
	set_signal(STOP_RESTORE, shell);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (print_signal_status(WTERMSIG(status)));
	if (cmd->redirs)
		cleanup_heredocs(cmd->redirs);
	return (1);
}

static void	handle_redirections(t_cmd *cmd, t_shell *shell)
{
	(void)shell;
	if (!cmd->redirs)
		return ;
	if (apply_redirections(cmd->redirs) == -1)
	{
		cleanup_shell(shell);
		exit(1);
	}
}

/* prepare_env_and_path is provided in a shared source file */

static void	exec_child_process(t_cmd *cmd, t_shell *shell)
{
	char	*cmd_path;
	char	**envp;

	handle_redirections(cmd, shell);
	install_child_signal_handlers();
	prepare_env_and_path(cmd, shell, &envp, &cmd_path);
	if (!cmd_path)
	{
		if (ft_strchr(cmd->argv[0], '/'))
			ft_putstr_fd("minishell: file not found: ", 2);
		else
			ft_putstr_fd("minishell: command not found: ", 2);
		ft_putendl_fd(cmd->argv[0], 2);
		free_char_arr(envp);
		cleanup_shell(shell);
		exit(127);
	}
	execve(cmd_path, cmd->argv, envp);
	cleanup_shell(shell);
	perror("minishell: execve");
	free_char_arr(envp);
	free(cmd_path);
	exit(126);
}
