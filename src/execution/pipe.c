/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchetoui <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 12:54:30 by mchetoui          #+#    #+#             */
/*   Updated: 2025/07/12 12:54:31 by mchetoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <main.h>

void	exec_pipe(t_cmd *cmd)
{
	int status;

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
	exit(0);
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
