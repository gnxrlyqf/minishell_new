/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchetoui <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 12:54:14 by mchetoui          #+#    #+#             */
/*   Updated: 2025/07/12 12:54:15 by mchetoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <main.h>

int	unset(char **args)
{
	t_env	*node;

	args++;
	while (*args)
	{
		node = get_env(data()->env, *args);
		if (node)
			free_node(&data()->env, node);
		args++;
	}
	return (0);
}
