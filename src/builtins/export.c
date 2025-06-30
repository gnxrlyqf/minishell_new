#include <main.h>

int	_isalpha(int c)
{
	return ((c >= 65 && c <= 90) || (c >= 97 && c <= 122));
}

int	_isalnum(int c)
{
	return (_isalpha(c) || (c >= 48 && c <= 57));
}

int validate_id(char *name)
{
	if (!_isalpha(*name) && *name != '_')
		return (0);
    name++;
    while (*name && *name != '=')
    {
        if (!_isalnum(*name) && *name != '_')
            return (0);
        name++;
    }
    return (1);
}

int export_add(char **args)
{
	char *key;
	t_env *node;

	while (*args)
	{
		if (!validate_id(*args))
			return (1);
		key = *args;
		while (**args && **args != '=')
			(*args)++;
		if (!**args)
			update_env(&g_shell.env, _strdup(key), NULL);
		else
		{
			**args = 0;
			update_env(&g_shell.env, _strdup(key), _strdup((*args) + 1));
		}
		args++;
	}
	return (0);
}

int export(char **args)
{
	t_env *curr;
	curr = g_shell.env;

	args++;
	if (args)
		return (export_add(args));
	while (curr)
	{
		printf("declare -x %s", curr->key);
		if (curr->value)
			printf("=\"%s\"", curr->value);
		printf("\n");
		curr = curr->next;
	}
	return (0);
}
