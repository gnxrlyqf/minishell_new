#include <main.h>

char	*mkpath(char *path, char *cmd)
{
	char	*out;
	int		i;

	if (*cmd == '/')
		return (NULL);
	out = malloc((size_t)(_strlen(path) + _strlen(cmd) + 2));
	if (!out)
	{
		throw_err(SYSCALL_FAIL, "malloc");
		;
	}
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
		throw_err(CMD_ENOENT, cmd);
	out = mkpath(curr->value, cmd);
	if (!access(out, F_OK | X_OK))
		return (out);
	free(out);
	return (cmd);
}

char	*check_path(char *path)
{
	struct stat st;
	
	if (stat(path, &st) == -1)
		return (NULL);
	if (S_ISDIR(st.st_mode))
		throw_err(IS_DIR, path);
	if (!access(path, F_OK | X_OK))
		return (path);
	throw_err(PERM_DENIED, path);		
	return (NULL);
}

char	*which(char *cmd, t_env *env)
{
	t_env 	*curr;
	char	*out;
	char	*path;
	char	*dup;
	
	path = check_path(cmd);
	if (path)
		return (path);
	curr = env;
	while (curr && _strcmp(curr->key, "PATH"))
		curr = curr->next;
	if (!curr)
		throw_err(CMD_ENOENT, cmd);
	dup = _strdup(curr->value);
	path = _strtok(dup, ":");
	while (path)
	{
		out = mkpath(path, cmd);
		if (!access(out, F_OK | X_OK))
			return (out);
		free(out);
		path = _strtok(NULL, ":");
	}
	return (free(dup), check_cwd(cmd, env));
}

void	exec(t_token *args, int size)
{
	char **envp;
	char **arr;
	char *path;

	if (!size)
		return ;
	arr = extract_args(args, size);
	if (!**arr)
		throw_err(CMD_ENOENT, "");
	path = which(*arr, g_shell.env);
	// free(*arr);
	*arr = path;
	// fprintf(2, "%s\n", *arr);
	envp = mkenvp(g_shell.env);
	execve(*arr, arr, envp);
	throw_err(CMD_ENOENT, *arr);
}
