/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_expansion.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchetoui <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 12:54:35 by mchetoui          #+#    #+#             */
/*   Updated: 2025/07/12 12:54:36 by mchetoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <main.h>

t_list	*add_node(t_list **head, void *value)
{
	t_list	*curr;
	t_list	*new;

	new = malloc(sizeof(t_list));
	if (!new)
		return (NULL);
	new->next = NULL;
	new->data = (void *)_strdup(value);
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
		free(temp->data);
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

int	fill_var(char *str, t_list **list, int *expanded)
{
	char	*varname;
	char	*value;
	int		varsize;

	if (*str == '?')
	{
		if (expanded)
			*expanded = 1;
		return (expand_status(list));
	}
	varname = _strddup(str, " $'\"");
	varsize = _strlen(varname);
	value = get_env_val(g_shell.env, varname);
	if (value)
	{
		while (*value)
			add_node(list, value++);
		free(varname);
	}
	if (expanded)
		*expanded = 1;
	return (varsize + 1);
}

char	*make_str(t_list *list)
{
	char	*str;
	int		size;
	t_list	*curr;
	int		i;

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
