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

void	wait_child_processes(int last)
{
	int		status;
	int		sig_flag;
	pid_t	wait_pid;

	sig_flag = 0;
	while (1)
	{
		wait_pid = wait(&status);
		if (wait_pid == -1)
			break ;
		if (WIFSIGNALED(status) && \
			(WTERMSIG(status) == SIGINT || WTERMSIG(status) == SIGQUIT))
			sig_flag = WTERMSIG(status);
	}
	if (sig_flag == SIGINT)
		write(1, "\n", 1);
	else if (sig_flag == SIGQUIT)
		write(2, "Quit (core dumped)\n", 19);
}
int	do_pipeline(t_cmd *pipeline)
{
	int	status;

	status = 0;
	while (pipeline->next)
	{
		cmd_pipe(pipeline);
		pipeline = pipeline->next;
	}
	// cmd(pipeline);
	// wait_child_processes(last);
	return (cmd(pipeline));
}

int	start(t_cmd *pipeline)
{
	int	fds[3];
	int	status;

	status = 0;
	fds[0] = dup(0);
	fds[1] = dup(1);
	fds[2] = dup(2);
	data()->status = do_pipeline(pipeline);
	dup2(fds[0], 0);
	dup2(fds[1], 1);
	dup2(fds[2], 2);
	close(fds[0]);
	close(fds[1]);
	close(fds[2]);
	return (data()->status);
}

void parent(int pid, int *status)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	waitpid(pid, status, 0);
	signal(SIGINT, foo);
	signal(SIGQUIT, SIG_IGN);
	if (WIFEXITED(*status))
		data()->status = WEXITSTATUS(*status);
	else if (WIFSIGNALED(*status))
	{
		if (WTERMSIG(*status) == SIGINT)
			write(1, "\n", 1);
		else if (WTERMSIG(*status) == SIGQUIT)
			write(2, "Quit (core dumped)\n", 19);
		if (WTERMSIG(*status) == SIGINT || WTERMSIG(*status) == SIGQUIT)
			data()->status = 128 + WTERMSIG(*status);
	}
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
	}
	else
		parent(pid, &status);
	return (data()->status);
}
