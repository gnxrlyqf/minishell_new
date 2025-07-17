/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   breakdown.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchetoui <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 12:54:25 by mchetoui          #+#    #+#             */
/*   Updated: 2025/07/12 12:54:26 by mchetoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <_printfd.h>
#include <main.h>

int	my_open_b(char *path, int flags, int del)
{
	int		fd;
	char	*ex;
	int		expanded;

	expanded = 0;
	ex = quotes_expand(path, &expanded);
	if (expanded && (!*ex || _strchr(ex, ' ') || _strchr(ex, '*')))
	{
		_printfd(2, "%s: ambiguous redirect\n", path);
		free(ex);
		return (-1);
	}
	if (!_strcmp(ex, "/dev/stdin"))
		return (0);
	if (!_strcmp(ex, "/dev/stdout"))
		return (1);
	fd = open(ex, flags, 0644);
	if (fd == -1)
		perror(ex);
	if (del)
		unlink(ex);
	free(ex);
	return (fd);
}

int	my_open(char *path, int flags, int del)
{
	int		fd;
	char	*ex;
	int		expanded;

	expanded = 0;
	ex = quotes_expand(path, &expanded);
	if (expanded && (!*ex || _strchr(ex, ' ') || _strchr(ex, '*')))
	{
		throw_err(AMBIG_REDIR, path);
		free(ex);
		exit(1);
	}
	if (!_strcmp(ex, "/dev/stdin"))
		return (0);
	if (!_strcmp(ex, "/dev/stdout"))
		return (1);
	fd = open(ex, flags, 0644);
	if (fd == -1)
		throw_err(OPEN_FAIL, ex);
	if (del)
		unlink(ex);
	free(ex);
	return (fd);
}

char	*mkfilename(char *path)
{
	char			*start;
	char			*out;
	unsigned long	address;
	int				i;

	start = "/tmp/.hd";
	address = (unsigned long)path;
	out = malloc(17);
	i = -1;
	while (++i < 8)
		out[i] = start[i];
	while (i < 16)
	{
		out[i] = (address % 10) + 48;
		address /= 10;
		i++;
	}
	out[i] = 0;
	return (out);
}

void	redir(t_redir *redir, int size)
{
	int		i;
	int		fd[2];
	char	*file;

	i = -1;
	fd[0] = 0;
	fd[1] = 1;
	while (++i < size)
	{
		file = redir[i].file;
		if (redir[i].type == Redirect_In || redir[i].type == Here_doc)
			fd[0] = my_open(file, O_RDONLY, (redir[i].type == Here_doc));
		if (redir[i].type == Redirect_Out || redir[i].type == Append)
			fd[1] = my_open(file, 1089 | (redir[i].type == 3) * 512, 0);
		if (fd[0] != 0)
		{
			dup2(fd[0], 0);
			close(fd[0]);
		}
		if (fd[1] != 1)
		{
			dup2(fd[1], 1);
			close(fd[1]);
		}
	}
}

int	redir_builtin(t_redir *r, int size, int i)
{
	int		fd[2];

	fd[0] = 0;
	fd[1] = 1;
	while (++i < size)
	{
		if (r[i].type == Redirect_In || r[i].type == Here_doc)
			fd[0] = my_open_b(r[i].file, O_RDONLY, (r[i].type == 5));
		if (r[i].type == Redirect_Out || r[i].type == Append)
			fd[1] = my_open_b(r[i].file, 1089 | (r[i].type == 3) * 512, 0);
		if (fd[0] == -1 || fd[1] == -1)
			return (1);
		if (fd[0] != 0)
		{
			dup2(fd[0], 0);
			close(fd[0]);
		}
		if (fd[1] != 1)
		{
			dup2(fd[1], 1);
			close(fd[1]);
		}
	}
	return (0);
}
