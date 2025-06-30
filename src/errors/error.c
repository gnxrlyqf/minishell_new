#include <main.h>

void throw_err(t_err code, char *data)
{
	static error_handler handlers[] = {
		NULL,
		empty_prompt,
		unexpected_token,
		syscall_fail,
		perm_denied,
		cmd_enoent,
		ambig_redir,
		is_dir
	};
	handlers[code](data);
}

void set_err(t_err code, char *data, int throw)
{
	g_shell.error->code = code;
	g_shell.error->data = _strdup(data);
	if (throw)
		throw_err(code, data);
}