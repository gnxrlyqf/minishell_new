#include <main.h>

int my_open_builtin(char *path, int flags)
{
	int fd;

	if (!*path || _strchr(path, ' ') || _strchr(path, '*'))
		throw_err(AMBIG_REDIR, path);
	fd = open(path, flags, 0644);
	if (fd == -1)
		perror(path);
	return (fd);
}

int my_open(char *path, int flags)
{
	int fd;

	if (!*path || _strchr(path, ' ') || _strchr(path, '*'))
		throw_err(AMBIG_REDIR, path);
	fd = open(path, flags, 0644);
	if (fd == -1)
		throw_err(OPEN_FAIL, path);
	return (fd);
}

char *mkfilename(char *path)
{
	char *start;
	char *out;
	int address;
	int i;

	start = "/tmp/.hd";
	address = (int)path;
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

char *do_heredoc(char *eof, int expand)
{
	int fd;
	char *file;
	char *line;

	file = mkfilename(eof);
	fd = open(file, O_WRONLY | O_CREAT, 0644);
	while (1)
	{
		line = readline("> ");
		if (!line || !_strncmp(line, eof, _strlen(eof)))
			break ;
		if (expand)
			line = quotes_expand(line);
		write(fd, line, _strlen(line));
		write(fd, "\n", 1);
	}
	close(fd);
	return (file);
}

void redir(t_redir *redir, int size)
{
	int i;
	int fd[2];
	char *file;

	i = -1;
	fd[0] = 0;
	fd[1] = 1;
	while (++i < size)
	{
		file = redir[i].file;
		if (redir[i].type == Redirect_In || redir[i].type == Here_doc)
			fd[0] = my_open(file, O_RDONLY);
		if (redir[i].type == Redirect_Out || redir[i].type == Append)
			fd[1] = my_open(file, 1089 | (redir[i].type == 3) * 512);
		if (redir[i].type == Here_doc)
			unlink(file);
	}
	dup2(fd[0], 0);
	dup2(fd[1], 1);
}

int redir_builtin(t_redir *redir, int size)
{
	int i;
	int fd[2];
	char *file;

	i = -1;
	fd[0] = 0;
	fd[1] = 1;
	while (++i < size)
	{
		file = redir[i].file;
		if (redir[i].type == Redirect_In || redir[i].type == Here_doc)
			fd[0] = my_open_builtin(file, O_RDONLY);
		if (redir[i].type == Redirect_Out || redir[i].type == Append)
			fd[1] = my_open_builtin(file, 1089 | (redir[i].type == 3) * 512);
		if (redir[i].type == Here_doc)
			unlink(file);
		if (fd[0] == -1 || fd[1] == -1)
			return (1);
	}
	dup2(fd[0], 0);
	dup2(fd[1], 1);
	return (0);
}

int my_dup2(int fd1, int fd2)
{
	dup2(fd1, fd2);
	if (fd1 != 0 && fd1 != 1)
		close(fd1);
	return (fd2);
}