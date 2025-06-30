#include <main.h>

int env(char **args)
{
	t_env *curr;

	(void)args;
	curr = g_shell.env;
	while (curr)
	{
		if (curr->value)
			printf("%s=%s\n", curr->key, curr->value);
		curr = curr->next;
	}
	return (0);
}