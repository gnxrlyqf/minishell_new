/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchetoui <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 12:54:12 by mchetoui          #+#    #+#             */
/*   Updated: 2025/07/12 12:54:13 by mchetoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <main.h>

int	pwd(char **args)
{
	char	*pwd;

	(void)args;
	pwd = _strdup(g_shell.chached_pwd);
	if (!pwd)
		pwd = _strdup(get_env_val(g_shell.env, "PWD"));
	if (!pwd)
		pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		perror("pwd: error retrieving current directory: getcwd: cannot \
			access parent directories");
		return (1);
	}
	printf("%s\n", pwd);
	free(pwd);
	return (0);
}
