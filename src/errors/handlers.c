#include <main.h>
#include <_printfd.h>

void unexpected_token(char *data)
{
	write(2, "Error: unexpected token: `", 26);
	write(2, data, _strlen(data));
	write(2, "'\n", 2);
}

void syscall_fail(char *data)
{
	free_pipeline(g_shell.pipeline);
	free_env(g_shell.env);
	free(data);
	exit(1);
}

void open_fail(char *data)
{
	perror(data);
	free_pipeline(g_shell.pipeline);
	free_env(g_shell.env);
	free(data);
	exit(1);
}

void file_enoent(char *data)
{
	_printfd(2, "%s: No such file or directory\n", data);
	free_pipeline(g_shell.pipeline);
	free_env(g_shell.env);
	exit(127);
}