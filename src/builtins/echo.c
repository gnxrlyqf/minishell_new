/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchetoui <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 12:53:37 by mchetoui          #+#    #+#             */
/*   Updated: 2025/07/12 12:53:44 by mchetoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <main.h>

int	echo(char **args)
{
	int	dash_n;

	dash_n = 0;
	args++;
	if (!*args)
	{
		printf("\n");
		return (0);
	}
	while (!_strcmp(*args, "-n"))
	{
		dash_n = 1;
		args++;
	}
	while (*args)
	{
		printf("%s", *args);
		args++;
		if (*args)
			printf(" ");
	}
	if (!dash_n)
		printf("\n");
	return (0);
}
