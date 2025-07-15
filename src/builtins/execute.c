/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchetoui <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 12:54:07 by mchetoui          #+#    #+#             */
/*   Updated: 2025/07/12 12:54:09 by mchetoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <main.h>

int	check_builtins(t_cmd *cmd)
{
	char			**args;
	int				i;
	static t_builtin	funcs[] = {
		echo, cd, pwd, export, unset, env, __exit
	};
	static char		*names[] = {
		"echo", "cd", "pwd", "export", "unset", "env", "exit"
	};

	if (!cmd->argcount)
		return (-1);
	args = cmd->args;
	i = 0;
	while (i < 7 && _strcmp(*args, names[i]))
		i++;
	if (i == 7)
		return (-1);
	if (cmd->redircount)
	{
		if (redir_builtin(cmd->redir, cmd->redircount))
			return (data()->status = 1, 1);
	}
	data()->status = funcs[i](args);
	return (data()->status);
}
