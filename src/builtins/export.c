/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchetoui <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 12:54:11 by mchetoui          #+#    #+#             */
/*   Updated: 2025/07/12 12:54:12 by mchetoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

int	validate_id(char *name)
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

int	export_add(char **args)
{
	char	*key;
	int		status;

	status = 0;
	while (*args)
	{
		if (!validate_id(*args))
		{
			_printfd(2, "export: %s: not a valid identifier\n", *(args++));
			status = 1;
			continue ;
		}
		key = *args;
		while (**args && **args != '=')
			(*args)++;
		if (!**args)
			update_env(&data()->env, _strdup(key), NULL);
		else
		{
			**args = 0;
			update_env(&data()->env, _strdup(key), _strdup((*args) + 1));
		}
		*(args++) = key;
	}
	return (status);
}

int	export(char **args)
{
	t_env	*curr;

	args++;
	if (*args)
		return (export_add(args));
	curr = data()->env;
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
