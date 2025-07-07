#include <main.h>

int _chdir(char *dir)
{
	char *oldpwd;
	char *pwd;

	if (!*dir)
		return (1);
	oldpwd = getcwd(NULL, 0);
	if (oldpwd)
		update_env(&g_shell.env, "OLDPWD", oldpwd);
	if (chdir(dir) == -1)
	{
		g_shell.status = 1;
		write(2, "cd: ", 4);
		write(2, dir, _strlen(dir));
		write(2, ": No such file or directory\n", 28);
		return (1);
	}
	free(dir);
	pwd = getcwd(NULL, 0);
	update_env(&g_shell.env, "PWD", pwd);
	if (g_shell.chached_pwd)
		free(g_shell.chached_pwd);
	g_shell.chached_pwd = _strdup(pwd);
	return (0);
}

int cd(char **args)
{
	char *dir;

	args++;
	if (!*args)
	{
		dir = quotes_expand("$HOME");
		return (_chdir(dir));
	}
	if (*(args + 1))
	{
		write(2, "cd: too many arguments\n", 23);
		return (1);
	}
	dir = quotes_expand(*args);
	return (_chdir(dir));
}