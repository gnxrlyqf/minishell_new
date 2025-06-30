#include <main.h>

int do_pipeline(t_cmd *pipeline)
{
	while (pipeline->next)
	{
		cmd_pipe(pipeline);
		pipeline = pipeline->next;
	}
	return (cmd(pipeline));
}

int start(t_cmd *pipeline)
{
	int fds[3];
	int status;

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
	return (g_shell.status);
}

int cmd(t_cmd *cmd)
{
	int pid;
	int status;

	status = check_builtins(cmd);
	if (status != -1)
		return (status);
	status = 0;
	pid = fork();
	if (pid == -1)
		throw_err(SYSCALL_FAIL, "fork");
	if (!pid)
	{
		if (cmd->redircount)
			redir(cmd->redir, cmd->redircount);
		if (cmd->argcount)
			exec(cmd->args, cmd->argcount);
	}
	else
		waitpid(pid, &status, 0);
	g_shell.status = status >> 8;
	return (g_shell.status);
}
