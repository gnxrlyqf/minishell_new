#include <main.h>

void	cmd_pipe(t_cmd *cmd)
{
	int		fdp[2];
	pid_t	pid;
	int		status;

	status = 0;
	if (pipe(fdp) == -1)
		throw_err(SYSCALL_FAIL, "pipe");
	pid = fork();
	if (pid == -1)
		throw_err(SYSCALL_FAIL, "fork");
	if (!pid)
	{
		close(fdp[0]);
		dup2(fdp[1], 1);
		if (cmd->redircount)
			redir(cmd->redir, cmd->redircount);
		if (cmd->argcount)
			exec(cmd->args);
		fprintf(2, "test\n");
	}
	else
	{
		close(fdp[1]);
		dup2(fdp[0], 0);
	}
}
