#include <main.h>

int echo(char **args)
{
	int dash_n;

	args++;
	if (!*args)
		return (write(1, "\n", 1));
	dash_n = !_strcmp(*args, "-n");
	args += dash_n;
	while (*args)
	{
		if (write(1, *args, _strlen(*args)) == -1)
			throw_err(SYSCALL_FAIL, "write");
		if (!dash_n)
			write(1, "\n", 1);
		args++;
	}
	return (1);
}