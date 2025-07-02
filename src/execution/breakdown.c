#include <main.h>
int my_open(char *path, int flags)
{
	int fd;

	if (!*path || _strchr(path, ' ') || _strchr(path, '*'))
		throw_err(AMBIG_REDIR, path);
	// if ((flags & O_RDONLY) && access(path, R_OK) == -1)
	// 	throw_err(PERM_DENIED, path);
	// if ((flags & O_WRONLY) && access(path, W_OK) == -1)
	// 	throw_err(PERM_DENIED, path);
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
		if (!line || !_strncmp(line, eof, _strlen(line) - 1))
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
	int in;
	int out;
	char *file;

	i = 0;
	in = 0;
	out = 1;
	while (i < size)
	{
		file = redir[i].file;
		if (redir[i].type == Redirect_In || redir[i].type == Here_doc)
			in = my_open(file, O_RDONLY);
		if (redir[i].type == Redirect_Out)
			out = my_open(file, O_WRONLY | O_TRUNC | O_CREAT);
		if (redir[i].type == Append)
			out = my_open(file, O_WRONLY | O_APPEND | O_CREAT);
		i++;
	}
	if (in != 0)
		dup2(in, 0);
	if (out != 1)
		dup2(out, 1);
}
