/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchetoui <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 18:16:02 by mchetoui          #+#    #+#             */
/*   Updated: 2025/07/17 18:16:03 by mchetoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <main.h>
#include <_printfd.h>

void	do_heredoc(char *file, char *eof, int expand)
{
	int		fd;
	char	*line;

	signal(SIGINT, heredoc_sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	fd = open(file, O_WRONLY | O_CREAT, 0644);
	free(file);
	while (1)
	{
		line = readline("> ");
		if (!line || !_strcmp(line, eof))
			break ;
		if (expand)
			line = quotes_expand(line, NULL);
		_printfd(fd, "%s\n", line);
		free(line);
	}
	if (!line)
		_printfd(1, "\nminishell: warning: here-document\
 delimited by end-of-file (wanted `%s')\n", eof);
	close(fd);
	free_lexer(data()->lexer);
	cleanup(15);
	exit(0);
}

void	hdparent(int pid, char *file)
{
	int	status;

	signal(SIGINT, SIG_IGN);
	waitpid(pid, &status, 0);
	signal(SIGINT, sigint_handler);
	if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
	{
		unlink(file);
		data()->status = 130;
	}
}

char	*hdoc(t_cmd *cmd, char *eof, int expand)
{
	t_shell	*shell;
	int		pid;
	char	*file;

	shell = data();
	file = mkfilename(eof);
	pid = fork();
	if (!pid)
	{
		if (shell->pipeline)
			shell->pipeline->next = cmd;
		else
			shell->pipeline = cmd;
		do_heredoc(file, eof, expand);
	}
	else
	{
		hdparent(pid, file);
		if (shell->status == 130)
		{
			shell->signal = 130;
			return (free(file), NULL);
		}
	}
	return (file);
}
