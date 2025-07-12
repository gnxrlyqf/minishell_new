/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchetoui <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 12:54:18 by mchetoui          #+#    #+#             */
/*   Updated: 2025/07/12 12:54:19 by mchetoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <main.h>

void	free_node(t_env **env, t_env *node)
{
	t_env	*prev;
	t_env	*curr;

	if (*env == node)
	{
		curr = (*env)->next;
		free((*env)->key);
		if ((*env)->value)
			free((*env)->value);
		free(*env);
		*env = curr;
		return ;
	}
	curr = *env;
	while (curr && curr != node)
	{
		prev = curr;
		curr = curr->next;
	}
	prev->next = curr->next;
	free(curr->key);
	if (curr->value)
		free(curr->value);
	free(curr);
}

void	free_env(t_env *env)
{
	t_env	*target;
	t_env	*curr;

	curr = env;
	while (curr)
	{
		target = curr;
		curr = curr->next;
		free(target->key);
		if (target->value)
			free(target->value);
		free(target);
	}
}
