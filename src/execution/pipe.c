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
		free_pipeline(g_shell.pipeline);
		free_env(g_shell.env);
		exit(status);
	}
	if (cmd->redircount)
		redir(cmd->redir, cmd->redircount);
	if (cmd->argcount)
		exec(cmd->args);
}

void	cmd_pipe(t_cmd *cmd)
{
	int		fdp[2];
	pid_t	pid;
	int		status;

	status = 0;
	pipe(fdp);
	pid = fork();
	if (pid == -1)
		throw_err(SYSCALL_FAIL, "fork");
	if (!pid)
	{
		close(fdp[0]);
		dup2(fdp[1], 1);
		exec_pipe(cmd);
	}
	else
	{
		close(fdp[1]);
		dup2(fdp[0], 0);
	}
}
