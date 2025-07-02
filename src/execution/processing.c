#include <main.h>

int	expand_status(t_list **list)
{
	int len;
	char *status;

	status = _itoa(g_shell.status);
	len = 0;
	while (*status)
	{
		len++;
		add_node(list, status++);
	}
	return (2);
}

char    *quotes(char *str)
{
	char    *result;

	int i;
	int j;
	int c;
	result = malloc(ft_strlen(str) + 1);
	i = 0;
	j = 0;
	c = 0;
	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '"') && c)
			c = str[i];
		else if (str[i] == c)
			c = 0;
		else
			result[j++] = str[i];
		i++;
	}
	result[j] = '\0';
	return (result);
}

char *quotes_expand(char *str)
{
	t_list *list;
	char *cpy;
	char *ret;
	int c;
	
	list = NULL;
	c = -1;
	cpy = str;
	while (*cpy)
	{
		if (*cpy == '$' && c != '\'' && *(cpy + 1) >= 48)
			cpy += fill_var(cpy + 1, &list);
		if ((*cpy == '\'' || *cpy == '"') && c == -1)
			c = *cpy;
		else if (*cpy == c)
			c = -1;
		else
			add_node(&list, cpy);
		cpy++;
	}
	ret = make_str(list);
	free_list(&list);
	return (ret);
}

char **extract_args(t_token *tokens, int size)
{
	int i;
	char **arr;

	arr = malloc(sizeof(char *) * (size + 1));
	arr[size] = NULL;
	i = -1;
	while (++i < size)
		arr[i] = quotes_expand(tokens[i].value);
	return (arr);
}
