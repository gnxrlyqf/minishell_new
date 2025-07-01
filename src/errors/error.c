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
