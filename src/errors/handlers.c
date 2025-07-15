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

void	unexpected_token(char *data)
{
	write(2, "Error: unexpected token: `", 26);
	write(2, data, _strlen(data));
	write(2, "'\n", 2);
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
