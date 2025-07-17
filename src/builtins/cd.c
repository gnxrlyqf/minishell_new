/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchetoui <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 12:53:24 by mchetoui          #+#    #+#             */
/*   Updated: 2025/07/12 12:53:30 by mchetoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <main.h>
#include <_printfd.h>

int	_chdir(char *dir)
{
	char	*oldpwd;
	char	*pwd;

	if (!*dir)
		return (1);
	oldpwd = getcwd(NULL, 0);
	if (oldpwd)
		update_env(&data()->env, "OLDPWD", oldpwd);
	if (chdir(dir) == -1)
	{
		data()->status = 1;
		_printfd(2, "cd: %s: No such file or directory\n", dir);
		return (1);
	}
	free(dir);
	pwd = getcwd(NULL, 0);
	update_env(&data()->env, "PWD", pwd);
	if (data()->chached_pwd)
		cleanup(4);
	data()->chached_pwd = _strdup(pwd);
	return (0);
}

int	cd(char **args)
{
	char	*dir;

	args++;
	if (!*args)
	{
		dir = quotes_expand("$HOME", NULL);
		return (_chdir(dir));
	}
	if (*(args + 1))
	{
		_printfd(2, "cd: too many arguments\n");
		return (1);
	}
	dir = quotes_expand(*args, NULL);
	return (_chdir(dir));
}
