#include <main.h>

void throw_err(t_err code, char *data)
{
	static error_handler handlers[] = {
		NULL,
		unexpected_token,
		syscall_fail,
		cmd_enoent,
		ambig_redir,
		is_dir,
		open_fail
	};
	handlers[code](data);
}
