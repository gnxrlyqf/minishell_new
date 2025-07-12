/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchetoui <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 12:54:21 by mchetoui          #+#    #+#             */
/*   Updated: 2025/07/12 12:54:22 by mchetoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <main.h>

void	throw_err(t_err code, char *data)
{
	static t_error_handler	handlers[] = {
		NULL,
		unexpected_token,
		syscall_fail,
		cmd_enoent,
		ambig_redir,
		is_dir,
		open_fail,
		file_enoent
	};

	handlers[code](data);
}
