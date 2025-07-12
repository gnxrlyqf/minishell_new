/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchetoui <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 12:54:10 by mchetoui          #+#    #+#             */
/*   Updated: 2025/07/12 12:54:10 by mchetoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <main.h>
#include <_printfd.h>

int	validate_status(char *str)
{
	if (_strchr("-+", *str))
		str++;
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (0);
		str++;
	}
	return (1);
}

int	__exit(char **args)
{
	int	status;

	status = g_shell.status;
	args++;
	if (*args)
	{
		if (validate_status(*args))
			status = (unsigned char)(ft_atoi(*args));
		else
		{
			_printfd(2, "exit: %s: numeric argument required\n", *args);
			status = 2;
		}
		args++;
		if (*args)
		{
			_printfd(2, "exit: too many arguments\n");
			status = 1;
		}
	}
	free_pipeline(g_shell.pipeline);
	free_env(g_shell.env);
	exit(status);
}
