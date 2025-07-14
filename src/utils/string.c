/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchetoui <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 12:55:47 by mchetoui          #+#    #+#             */
/*   Updated: 2025/07/12 12:55:48 by mchetoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <main.h>

char	*_strtok(char *str, char *delims)
{
	int			i;
	static char	*save;

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
	{
		free(new);
		return (NULL);
	}
	i = -1;
	while (src[++i])
		new[i] = src[i];
	new[i] = '\0';
	return (new);
}
