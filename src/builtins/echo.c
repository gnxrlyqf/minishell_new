#include <main.h>

int echo(char **args)
{
	int dash_n;

	dash_n = 0;
	args++;
	if (!*args)
		return (printf("\n"));
	while (!_strcmp(*args, "-n"))
	{
		dash_n = 1;	
		args++;
	}
	while (*args)
	{
		printf("%s ", *args);
		args++;
	}
	if (!dash_n)
		printf("\n");
	return (0);
}