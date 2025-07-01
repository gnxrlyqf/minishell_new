#include <main.h>

char	*max_str(char *a, char *b)
{
	if (a > b)
		return (a);
	return (b);
}

int skip(char *str, int i, char c, int rev)
{
	int subshell;
	int stop;

	stop = _strlen(str) * !rev - rev;
	if (c == '\'' || c == '"')
	{
		while (i != stop && str[i] != c)
			i += 1 - 2 * rev;		
		return (i);
	}
	if (c == ')' || c == '(')
	{
		subshell = 1 - 2 * (c == ')');
		while (i != stop && subshell)
		{
			subshell += (str[i] == ')' || str[i] == '(') - 2 * (str[i] == ')');
			i += 1 - 2 * rev;
		}
		return (i);
	}
	while (i != stop && str[i] == c)
		i += 1 - 2 * rev;
	return (i);
}

int wc(char *str, char c)
{
	int i;
	int count;

	count = 0;
	i = 0;
	while (str[i] == c)
		i++;
	while (str[i])
	{
		count++;
		while (str[i] && str[i] != c)
		{
			if (_strchr("\"'(", str[i]))
				i = skip(str, i + 1, str[i], 0);
			else
				i++;
		}
		while (str[i] == c)
			i++;
	}
	return (count);
}

int is_empty(char *str)
{
	if (!str)
		return (1);
	while (*str && *str == ' ')
		str++;
	if (!*str)
		return (1);
	return (0);
}

void init_shell(char **envp)
{
	g_shell.env = init_env(envp);
	g_shell.status = 0;
	g_shell.pipeline = NULL;
}