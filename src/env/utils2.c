#include <main.h>

void free_node(t_env **env, t_env *node)
{
	t_env *prev;
	t_env *curr;

	if (*env == node)
	{
		curr = (*env)->next;
		free((*env)->key);
		if ((*env)->value)
			free((*env)->value);
		free(*env);
		*env = curr;
		return ;
	}
	curr = *env;
	while (curr && curr != node)
	{
		prev = curr;
		curr = curr->next;
	}
	prev->next = curr->next;
	free(curr->key);
	if (curr->value)
		free(curr->value);
	free(curr);
}
