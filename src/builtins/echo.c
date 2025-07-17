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

int	check_flags(char **args, int *dash_n)
{
	int		i;
	char	*arg;

	i = 0;
	while (*args)
	{
		arg = *args;
		if (*arg != '-')
			return (i);
		arg++;
		while (*arg)
		{
			if (*arg != 'n')
				return (i);
			arg++;
		}
		i++;
		*dash_n = 1;
		args++;
	}
	return (i);
}

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
	args += check_flags(args, &dash_n);
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
