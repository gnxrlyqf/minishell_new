#include <main.h>

int pwd(char **args)
{
	char *pwd;

	(void)args;
	pwd = getcwd(NULL, 0);
	if (pwd == NULL)
	{
		perror("pwd: error retrieving current directory: getcwd: cannot \
			access parent directories");
		return (1);
	}
	printf("%s\n", pwd);
	return (0);
}