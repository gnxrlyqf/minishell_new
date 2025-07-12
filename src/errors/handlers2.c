#include <main.h>
#include <errno.h>
#include <_printfd.h>

void ambig_redir(char *data)
{
	_printfd(2, "%s: ambiguous redirect\n", data);
	free_pipeline(g_shell.pipeline);
	free_env(g_shell.env);
}

void cmd_enoent(char *data)
{
	_printfd(2, "%s: command not found\n", data);
	free_pipeline(g_shell.pipeline);
	free_env(g_shell.env);
	free(data);
	exit(127);
}

void is_dir(char *data)
{
	_printfd(2, "%s: is a directory\n", data);
	free_pipeline(g_shell.pipeline);
	free_env(g_shell.env);
	free(data);
	exit(126);
}