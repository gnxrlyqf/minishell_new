#include <main.h>

int	_strlen(char *str)
{
	int	len;

	len = 0;
	if (!str)
		return (len);
	while (*(str++))
		len++;
	return (len);
}

int	_strncmp(char *s1, char *s2, unsigned int n)
{
	unsigned int	i;

	i = 0;
	while ((*s1 && (*s1 == *s2)) && i < n)
	{
		s1++;
		s2++;
		i++;
	}
	if (i == n)
		return (0);
	return (*s1 - *s2);
}

char	*_strndup(char *str, char *set)
{
	int		size;
	char	*new;

	if (!str || !set)
		return (NULL);
	size = 0;
	while (*str && !_strchr(set, *str))
	{
		str++;
		size++;
	}
	new = malloc(size + 1);
	new += size;
	*new = 0;
	while (size--)
		*(--new) = *(--str);
	return (new);
}


int	_strcmp(char *s1, char *s2)
{
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return (*s1 - *s2);
}
