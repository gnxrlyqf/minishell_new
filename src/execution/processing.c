#include <main.h>

char *quotes_expand(char *str)
{
	t_list *list;
	char *cpy;
	char *ret;
	int c;
	
	list = NULL;
	c = 0;
	cpy = str;
	while (*cpy)
	{
		if (*cpy == '$' && c != '\'' && *(cpy + 1) >= 48)
			cpy += fill_var(cpy + 1, &list);
		if ((*cpy == '\'' || *cpy == '"') && c == 0)
			c = *cpy;
		else if (*cpy == c)
			c = 0;
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
	// free(args->members);
}
