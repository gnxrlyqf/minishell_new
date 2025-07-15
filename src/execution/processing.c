/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchetoui <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 12:54:31 by mchetoui          #+#    #+#             */
/*   Updated: 2025/07/12 12:54:34 by mchetoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <main.h>
#include <_printfd.h>

int	expand_status(t_list **list)
{
	char	*status;
	char	*cpy;

	status = _itoa(g_shell.status);
	cpy = status;
	while (*status)
		add_node(list, status++);
	status = cpy;
	free(status);
	return (2);
}

char	*quotes(char *str)
{
	char	*result;
	int		i;
	int		j;
	int		c;

	result = malloc(ft_strlen(str) + 1);
	i = 0;
	j = 0;
	c = 0;
	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '"') && c)
			c = str[i];
		else if (str[i] == c)
			c = 0;
		else
			result[j++] = str[i];
		i++;
	}
	result[j] = '\0';
	return (result);
}

int		expandable(char *str)
{
	str++;
	if (!*str)
		return (0);
	if (_strchr("'\"", *str))
		return (1);
	if (!_isalpha(*str) && *str != '_' && *str != '?')
		return (0);
	return (1);
}

char	*quotes_expand(char *str, int *expanded)
{
	t_list	*list;
	char	*ret;
	int		c;

	list = NULL;
	c = -1;
	ret = str;
	while (*str)
	{
		if (*str == '$' && c != '\'' && expandable(str))
		{
			str += fill_var(str + 1, &list, expanded);
			continue ;
		}
		if ((*str == '\'' || *str == '"') && c == -1)
			c = *str;
		else if (*str == c)
			c = -1;
		else
			add_node(&list, str);
		if (!*str)
			break ;
		str++;
	}
	ret = make_str(list);
	free_list(&list);
	return (ret);
}


char	*do_heredoc(char *eof, int expand, int *hdsigint)
{
	int		pid;
	int		fd;
	char	*file;
	char	*line;
	int		status;

	file = mkfilename(eof);
	fd = open(file, O_WRONLY | O_CREAT, 0644);
	pid = fork();
	if (pid)
	{
		waitpid(pid, &status, 0);
		setup_interactive_signals();
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		{
			*hdsigint = 1;
			close(fd);
			unlink(file);
			free(file);
			return NULL;
		}
	}
	else
	{
		setup_heredoc_signals();
		while (1)
		{
			line = readline("> ");
			if (!line || !_strncmp(line, eof, _strlen(eof)))
				break ;
			if (expand)
				line = quotes_expand(line, NULL);
			_printfd(fd, "%s\n", line);
		}
		if (!line && g_shell.status != 130)
			_printfd(1, "minishell: warning: here-document\
		delimited by end-of-file (wanted `%s')\n", eof);
		close(fd);
		exit(0);
	}
	return (close(fd), file);
}
