/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchetoui <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 12:54:24 by mchetoui          #+#    #+#             */
/*   Updated: 2025/07/12 12:54:25 by mchetoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <main.h>
#include <_printfd.h>

void	unexpected_token(char *str)
{
	if (!*str)
		str = "newline";
	_printfd(2, "minishell: syntax error near unexpected token `%s'\n", str);
	data()->status = 2;
}

void	syscall_fail(char *data)
{
	cleanup(15);
	free(data);
	exit(1);
}

void	open_fail(char *data)
{
	perror(data);
	cleanup(15);
	free(data);
	exit(1);
}

void	file_enoent(char *data)
{
	_printfd(2, "%s: No such file or directory\n", data);
	cleanup(15);
	exit(127);
}
