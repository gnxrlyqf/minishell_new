/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchetoui <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 12:55:43 by mchetoui          #+#    #+#             */
/*   Updated: 2025/07/12 12:55:43 by mchetoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <main.h>

void	free_arr(char **arr, int size)
{
	int	i;

	i = 0;
	while (i < size)
		free(arr[i++]);
	free(arr);
}

void	free_node_cmd(t_cmd *cmd)
{
	int	i;

	if (cmd->argcount)
		free_arr(cmd->args, cmd->argcount);
	if (cmd->redircount)
	{
		i = 0;
		while (i < cmd->redircount)
			free(cmd->redir[i++].file);
		free(cmd->redir);
	}
	free(cmd);
}

void	free_pipeline(t_cmd *pipeline)
{
	t_cmd	*target;
	t_cmd	*curr;

	curr = pipeline;
	while (curr)
	{
		target = curr;
		curr = curr->next;
		free_node_cmd(target);
	}
}
