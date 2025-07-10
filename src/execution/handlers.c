#include <main.h>

int do_pipeline(t_cmd *pipeline)
{
	int status;

	status = 0;
	while (pipeline->next)
	{
		cmd_pipe(pipeline);
		pipeline = pipeline->next;
	}
	// while (waitpid(-1, &status, 0) != -1)
	// {
	// 	if (WIFSIGNALED(status) && status == 2)
	// 		g_shell.status = 130;
	// 	g_shell.status = status >> 8;
	// }
	// temporarily removed this part as it was causing issues during some tests, needs fixing
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

void cleanup_hd(t_redir *redir, int size)
{
	int i;

	i = -1;
	while (++i < size)
	{
		if (redir->type == Here_doc)
			unlink(redir->file);
	}
}

int cmd(t_cmd *cmd)
{
	int pid;
	int status;

	status = check_builtins(cmd);
	if (status != -1)
		return (status);
	status = 0;
	setup_exec_signals(); // FOR SIGNALS !!
	pid = fork();
	if (pid == -1)
		throw_err(SYSCALL_FAIL, "fork");
	if (!pid)
	{
		reset_signals_in_child(); // FOR SIGNALS !!
		if (cmd->redircount)
			redir(cmd->redir, cmd->redircount);
		if (cmd->argcount)
			exec(cmd->args);
	}
	else
		waitpid(pid, &status, 0);
	g_shell.in_execution = false; // FOR SIGNALS !!
	g_shell.status = status >> 8;
	return (g_shell.status);
}
