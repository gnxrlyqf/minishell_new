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

int	do_pipeline(t_cmd *pipeline)
{
	int	status;

	status = 0;
	while (pipeline->next)
	{
		cmd_pipe(pipeline);
		pipeline = pipeline->next;
	}
	// while (waitpid(-1, &status, 0) != -1)
	// {
	// 	if (WIFSIGNALED(status) && status == 2)
	// 		data()->status = 130;
	// 	data()->status = status >> 8;
	// }
	// temporarily removed this part as it was causing issues during some tests, needs fixing
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
	do_pipeline(pipeline);
	dup2(fds[0], 0);
	dup2(fds[1], 1);
	dup2(fds[2], 2);
	close(fds[0]);
	close(fds[1]);
	close(fds[2]);
	return (data()->status);
}

int	cmd(t_cmd *cmd)
{
	int	pid;
	int	status;

	status = check_builtins(cmd);
	if (status != -1)
		return (status);
	status = 0;
	// setup_parent_signals();
	pid = fork();
	if (pid == -1)
		throw_err(SYSCALL_FAIL, "fork");
	if (!pid)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		// setup_child_signals();
		if (cmd->redircount)
			redir(cmd->redir, cmd->redircount);
		if (cmd->argcount)
			exec(cmd->args);
	}
	else
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		waitpid(pid, &status, 0);
		signal(SIGINT, foo);
		signal(SIGQUIT, SIG_IGN);
		// setup_interactive_signals();
	}
	data()->status = status >> 8;
	return (data()->status);
}
