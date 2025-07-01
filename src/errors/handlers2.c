#include <main.h>

void ambig_redir(char *data)
{
	(void)data;
}

void empty_prompt(char *data)
{
	
}

void cmd_enoent(char *data)
{
	write(2, data, _strlen(data));
	write(2, ": command not found\n", 21);
	free_pipeline(g_shell.pipeline);
	free_env(g_shell.env);
	free(data);
	exit(127);
}

void perm_denied(char *data)
{
	
}

void is_dir(char *data)
{
	write(2, data, _strlen(data));
	write(2, ": is a directory\n", 17);
	free_pipeline(g_shell.pipeline);
	free_env(g_shell.env);
	free(data);
	exit(126);
}