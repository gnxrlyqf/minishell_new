#include <main.h>

int echo(char **args)
{
	int dash_n;

	dash_n = 0;
	args++;
	if (!*args)
		return (write(1, "\n", 1));
	while (!_strcmp(*args, "-n"))
	{
		dash_n = 1;	
		args++;
	}
	while (*args)
	{
		if (write(1, *args, _strlen(*args)) == -1)
			throw_err(SYSCALL_FAIL, "write");
		args++;
	}
	if (!dash_n)
		write(1, "\n", 1);
	return (0);
}