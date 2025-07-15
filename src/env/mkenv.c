/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mkenv.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchetoui <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 12:54:17 by mchetoui          #+#    #+#             */
/*   Updated: 2025/07/12 12:54:18 by mchetoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <main.h>

char	*mkvar(t_env *env)
{
	int		len;
	char	*ret;
	char	*key;
	char	*val;

	key = env->key;
	val = env->value;
	len = _strlen(key) + _strlen(val) + 1;
	ret = malloc(sizeof(char) * (len + 1));
	ret[len] = 0;
	while (*key)
		*(ret++) = *(key++);
	*(ret++) = '=';
	while (*val)
		*(ret++) = *(val++);
	ret -= len;
	return (ret);
}

char	**mkenvp(t_env *env, int *envsize)
{
	int		size;
	t_env	*curr;
	char	**envp;

	size = env_len(env);
	envp = malloc(sizeof(char *) * (size + 1));
	curr = env;
	while (curr)
	{
		if (curr->value)
		{
			*envp = mkvar(curr);
			envp++;
		}
		curr = curr->next;
	}
	*envsize = size;
	*envp = NULL;
	envp -= size;
	return (envp);
}
