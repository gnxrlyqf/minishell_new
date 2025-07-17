/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchetoui <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 12:54:27 by mchetoui          #+#    #+#             */
/*   Updated: 2025/07/12 12:54:28 by mchetoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <main.h>
#include <_printfd.h>
#include <errno.h>

char	*mkpath(char *path, char *cmd)
{
	char	*out;
	int		i;

	if (*cmd == '/')
		return (NULL);
	out = malloc((size_t)(_strlen(path) + _strlen(cmd) + 2));
	if (!out)
		throw_err(SYSCALL_FAIL, "malloc");
	i = 0;
	while (*path)
		out[i++] = *(path++);
	out[i++] = '/';
	while (*cmd)
		out[i++] = *(cmd++);
	out[i] = 0;
	return (out);
}

char	*check_cwd(char *cmd, t_env *env)
{
	struct stat	st;
	t_env		*curr;
	char		*out;

	curr = get_env(env, "PWD");
	if (!curr)
		return (_strdup(cmd));
	out = mkpath(curr->value, cmd);
	stat(out, &st);
	if (!access(out, F_OK | X_OK) && !S_ISDIR(st.st_mode))
		return (out);
	free(out);
	return (_strdup(cmd));
}

char	*check_path(char *path)
{
	struct stat	st;

	if (ft_strchr(path, '/'))
	{
		if (stat(path, &st) == -1)
			throw_err(FILE_ENOENT, path);
		if (S_ISDIR(st.st_mode))
			throw_err(IS_DIR, _strdup(path));
		if (!access(path, F_OK | X_OK))
			return (_strdup(path));
		_printfd(2, "%s: %s\n", path, strerror(errno));
		cleanup(3);
		exit(126);
	}
	return (NULL);
}

char	*which(char *cmd, t_env *env)
{
	char	*out;
	char	*path;
	char	*dup;

	path = check_path(cmd);
	if (path)
		return (path);
	dup = _strdup(get_env_val(env, "PATH"));
	if (!dup)
		return (cmd);
	path = _strtok(dup, ":");
	while (path)
	{
		out = mkpath(path, cmd);
		if (!access(out, F_OK | X_OK))
		{
			free(dup);
			return (out);
		}
		free(out);
		path = _strtok(NULL, ":");
	}
	free(dup);
	return (check_cwd(cmd, env));
}

void	exec(char **args)
{
	char	**envp;
	char	*path;
	int		envsize;
	t_shell	*shell;

	if (!**args)
		throw_err(CMD_ENOENT, _strdup(""));
	shell = data();
	path = which(*args, shell->env);
	envp = mkenvp(shell->env, &envsize);
	execve(path, args, envp);
	free_arr(envp, envsize);
	throw_err(CMD_ENOENT, path);
}
