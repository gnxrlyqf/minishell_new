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

	if (cmd->redircount)
		redir(cmd->redir, cmd->redircount);
	if (!cmd->argcount)
		return (-1);
	args = extract_args(cmd->args, cmd->argcount);
	i = 0;
	while (i < 7 && _strcmp(args[0], names[i]))
		i++;
	if (i == 7)
		return (-1);
	return (funcs[i](args));
}
