#include <main.h>

int __exit(char **args)
{
	int status;

	status = 0;
	if (args[1])
		status = ft_atoi(args[1]);
	free_pipeline(g_shell.pipeline);
	free_env(g_shell.env);
	exit(status);
}