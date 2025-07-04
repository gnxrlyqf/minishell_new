#include <main.h>

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
	t_env *curr;
	char	*out;

	curr = env;
	while (curr && _strcmp(curr->key, "PWD"))
		curr = curr->next;
	if (!curr)
		return (_strdup(cmd));
	out = mkpath(curr->value, cmd);
	if (!access(out, F_OK | X_OK))
		return (out);
	free(out);
	return (_strdup(cmd));
}

char	*check_path(char *path)
{
	struct stat st;
	
	if (stat(path, &st) == -1)
		return (NULL);
	if (S_ISDIR(st.st_mode))
		throw_err(IS_DIR, _strdup(path));
	if (!access(path, F_OK | X_OK))
		return (_strdup(path));
	write(2, path, _strlen(path));
	write(2, ": Permission denied\n", 21);
	free_pipeline(g_shell.pipeline);
	free_env(g_shell.env);
	exit(126);
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
	char **envp;
	char *path;
	int envsize;

	if (!**args)
		throw_err(CMD_ENOENT, "");
	path = which(*args, g_shell.env);
	envp = mkenvp(g_shell.env, &envsize);
	execve(path, args, envp);
	free_arr(envp, envsize);
	throw_err(CMD_ENOENT, path);
}
