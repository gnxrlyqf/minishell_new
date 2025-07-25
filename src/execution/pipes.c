/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchetoui <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 12:54:29 by mchetoui          #+#    #+#             */
/*   Updated: 2025/07/12 12:54:29 by mchetoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <main.h>

int	wait_children(int last)
{
	int		last_status;
	int		status;
	int		sig_flag;
	pid_t	wait_pid;

	last_status = 0;
	sig_flag = 0;
	while (1)
	{
		wait_pid = wait(&status);
		if (wait_pid == -1)
			break ;
		if (WIFSIGNALED(status) && (WTERMSIG(status) == SIGINT
				|| WTERMSIG(status) == SIGQUIT))
			sig_flag = WTERMSIG(status);
		if (wait_pid == last)
			last_status = status;
	}
	if (sig_flag == SIGINT)
		write(1, "\n", 1);
	else if (sig_flag == SIGQUIT)
		write(2, "Quit (core dumped)\n", 19);
	if (WIFSIGNALED(last_status))
		return (128 + WTERMSIG(last_status));
	return (WEXITSTATUS(last_status));
}

int	do_pipeline(t_cmd *pipeline)
{
	int	status;
	int	last;

	while (pipeline->next)
	{
		cmd_pipe(pipeline);
		pipeline = pipeline->next;
	}
	last = cmd(pipeline);
	status = wait_children(last);
	return (status);
}

int	cmd(t_cmd *cmd)
{
	int	pid;
	int	status;

	status = check_builtins(cmd);
	if (status != -1)
		return (status);
	status = 0;
	pid = fork();
	if (pid == -1)
		throw_err(SYSCALL_FAIL, "fork");
	if (!pid)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if (cmd->redircount)
			redir(cmd->redir, cmd->redircount);
		if (cmd->argcount)
			exec(cmd->args);
		cleanup(15);
		exit(0);
	}
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	return (pid);
}

void	cmd_pipe(t_cmd *cmd)
{
	int		fdp[2];
	pid_t	pid;

	pipe(fdp);
	pid = fork();
	if (pid == -1)
		throw_err(SYSCALL_FAIL, "fork");
	if (!pid)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		close(fdp[0]);
		dup2(fdp[1], 1);
		exec_pipe(cmd);
	}
	else
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		close(fdp[1]);
		dup2(fdp[0], 0);
	}
}

void	exec_pipe(t_cmd *cmd)
{
	int	status;

	status = check_builtins(cmd);
	if (status != -1)
	{
		cleanup(3);
		exit(status);
	}
	if (cmd->redircount)
		redir(cmd->redir, cmd->redircount);
	if (cmd->argcount)
		exec(cmd->args);
	cleanup(15);
	exit(0);
}
