#include <main.h>

int unset(char **args)
{
	t_env *node;

	args++;
	while (*args)
	{
		node = get_env(g_shell.env, *args);
		if (node)
			free_node(&g_shell.env, node);
		args++;
	}
	return (0);
}
