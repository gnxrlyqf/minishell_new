#include <main.h>

int check_builtins(t_cmd *cmd)
{
	char			**args;
	int 			i;
	static builtin	funcs[] = {
		echo, cd, pwd, export, unset, env, __exit
	};
	static char		*names[] = {
		"echo", "cd", "pwd", "export", "unset", "env", "exit"
	};

	if (!cmd->argcount)
		return (-1);
	args = cmd->args;
	i = 0;
	while (i < 7 && _strcmp(*args, names[i]))
		i++;
	if (i == 7)
		return (-1);
	if (cmd->redircount)
		redir(cmd->redir, cmd->redircount);
	g_shell.status = funcs[i](args);
	return (g_shell.status);
}
