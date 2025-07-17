/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchetoui <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 12:54:31 by mchetoui          #+#    #+#             */
/*   Updated: 2025/07/12 12:54:34 by mchetoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <_printfd.h>
#include <lexer.h>
#include <main.h>

int	expand_status(t_list **list)
{
	char	*status;
	char	*cpy;

	status = _itoa(data()->status);
	cpy = status;
	while (*status)
		add_node(list, status++);
	status = cpy;
	free(status);
	return (2);
}

char	*quotes(char *str)
{
	char	*result;
	int		i;
	int		j;
	int		c;

	result = malloc(ft_strlen(str) + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	c = 0;
	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '"') && c == 0)
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

int	expandable(char *str, int c)
{
	str++;
	if (!*str)
		return (0);
	if (_strchr("'\"", *str) && c != '"')
		return (1);
	if (!_isalpha(*str) && *str != '_' && *str != '?')
		return (0);
	return (1);
}

char	*quotes_expand(char *str, int *expanded)
{
	t_list	*list;
	char	*ret;
	int		c;

	list = NULL;
	c = -1;
	ret = str;
	while (*str)
	{
		if (*str == '$' && c != '\'' && expandable(str, c))
		{
			str += fill_var(str + 1, &list, expanded);
			continue ;
		}
		if ((*str == '\'' || *str == '"') && c == -1)
			c = *str;
		else if (*str == c)
			c = -1;
		else
			add_node(&list, str);
		if (!*str)
			break ;
		str++;
	}
	return (ret = make_str(list), free_list(&list), ret);
}
