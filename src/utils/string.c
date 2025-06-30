#include <main.h>

char	*_strrstr_skip(char *str, char *sub)
{
	int	i;
	int sublen;

	sublen = _strlen(sub);
	i = _strlen(str);
	while (--i >= 0)
	{
		if (_strchr("'\")", str[i]))
			i = skip(str, i - 1, str[i], 1);
		if (!_strncmp(str + i, sub, sublen))
			return (str + i);
	}
	return (NULL);
}

char	*_strstr_skip(char *str, char *sub)
{
	int	i;
	int sublen;
	int strlen;

	sublen = _strlen(sub);
	strlen = _strlen(str);
	i = -1;
	while (++i < strlen)
	{
		if (_strchr("'\"(", str[i]))
			i = skip(str, i + 1, str[i], 0);
		if (!_strncmp(str + i, sub, sublen))
			return (str + i);
	}
	return (NULL);
}

char	*_strtok(char *str, char *delims)
{
	int	i;
	static char *save;

	i = 0;
	if (!str)
		str = save;
	while (*str && _strchr(delims, *str))
		str++;
	if (!*str)
		return (NULL);
	while (*str && !_strchr(delims, *str))
	{
		i++;
		str++;
	}
	save = str + 1 * (*str != 0);
	*str = 0;
	return (str - i);
}

char	*_strchr(char *str, char c)
{
	while (*str && *str != c)
		str++;
	if (!*str)
		return (NULL);
	return (str);
}

char	*_strdup(char *src)
{
	int		i;
	char	*new;

	new = malloc((_strlen(src) + 1) * sizeof(char));
	if (!src || !new)
		return (NULL);
	i = -1;
	while (src[++i])
		new[i] = src[i];
	new[i] = '\0';
	return (new);
}
