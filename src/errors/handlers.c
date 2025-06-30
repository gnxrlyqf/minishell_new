#include <main.h>

void unexpected_token(char *data)
{
	write(2, "Error: unexpected token: `", 26);
	write(2, data, _strlen(data));
	write(2, "'\n", 2);
}

void syscall_fail(char *data)
{
	write(2, "Error: syscall fail: ", 22);
	write(2, data, _strlen(data));
	write(2, "\n", 1);
}
