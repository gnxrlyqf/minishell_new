#include <main.h>

t_env *init_env(char **envp)
{
	t_env *env;
	char *var;
	char *name;
	char *value;

	env = NULL;
	while (*envp)
	{
		name = *envp;
		var = *envp;
		while (*var != '=')
			var++;
		*var = 0;
		value = var + 1;
		add_node_env(&env, _strdup(name), _strdup(value));
		envp++;
	}
	return (env);
}
