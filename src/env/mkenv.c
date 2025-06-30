#include <main.h>

char *mkvar(t_env *env)
{
	int		len;
	char	*ret;
	char	*key;
	char	*val;

	key = env->key;
	val = env->value;
	len = _strlen(env->key) + _strlen(env->value) + 1;
	ret = malloc(sizeof(char) * (len + 1));
	while (*key)
		*(ret++) = *(key++);
	*(ret++) = '=';
	while (*val)
		*(ret++) = *(val++);
	*ret = 0;
	ret -= len;
	return (ret);
}

char **mkenvp(t_env *env)
{
	int size;
	t_env *curr;
	char **envp;

	size = env_len(env);
	envp = malloc(sizeof(char *) * (size + 1));
	curr = env;
	while (curr)
	{
		*envp = mkvar(curr);
		envp++;
		curr = curr->next;
	}
	*envp = NULL;
	envp -= size;
	return (envp);
}