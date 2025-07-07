#include <main.h>
#include <_printfd.h>

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

	while (*args)
	{
		if (!validate_id(*args))
		{
			_printfd(2, "export: %s: not a valid identifier\n", *args);
			args++;
			continue ;
		}
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
		*args = key;
		args++;
	}
	return (0);
}

int export(char **args)
{
	t_env *curr;
	
	args++;
	if (*args)
		return (export_add(args));
	curr = g_shell.env;
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
