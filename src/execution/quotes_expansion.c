#include <main.h>

t_list	*add_node(t_list **head, void *value)
{
	t_list	*curr;
	t_list	*new;

	new = malloc(sizeof(t_list));
	if (!new)
		return (NULL);
	new->next = NULL;
	new->data = value;
	if (!*head)
	{
		*head = new;
		return (new);
	}
	curr = *head;
	while (curr->next)
		curr = curr->next;
	curr->next = new;
	return (new);
}

void	free_list(t_list **head)
{
	t_list	*current;
	t_list	*temp;

	if (!head)
		return ;
	current = *head;
	while (current)
	{
		temp = current;
		current = current->next;
		free(temp);
	}
	*head = NULL;
}

int	list_len(t_list *list)
{
	int		i;
	t_list	*curr;

	if (!list)
		return (0);
	i = 0;
	curr = list;
	while (curr)
	{
		i++;
		curr = curr->next;
	}
	return (i);
}

int fill_var(char *str, t_list **list)
{
	t_env *curr;
	char *varname;
	char *value;
	int varsize;

	if (_strchr(" '\"", *str))
		return (1);
	varname = _strndup(str, " $'\"");
	varsize = _strlen(varname);
	curr = g_shell.env;
	while (curr && _strcmp(curr->key, varname))
		curr = curr->next;
	if (curr)
	{
		value = curr->value;
		while (*value)
			add_node(list, value++);
		free(varname);
	}
	return (varsize + 1);
}

char *make_str(t_list *list)
{
	char *str;
	int size;
	t_list *curr;
	int i;

	size = list_len(list);
	str = malloc(size + 1);
	str[size] = 0;
	i = 0;
	curr = list;
	while (curr)
	{
		*(str + i) = *(char *)(curr->data);
		curr = curr->next;
		i++;
	}
	return (str);
}
