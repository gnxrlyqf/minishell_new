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

#include <_printfd.h>
#include <lexer.h>
#include <main.h>

int	expand_status(t_list **list)
{
	char	*status;
	char	*cpy;

	status = _itoa(data()->status);
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
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	c = 0;
	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '"') && c == 0)
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

int	expandable(char *str, int c)
{
	str++;
	if (!*str)
		return (0);
	if (_strchr("'\"", *str) && c != '"')
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
		if (*str == '$' && c != '\'' && expandable(str, c))
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

// int	setup(t_cmd *cmd, char *file)
// {
// 	int fd;

// 	signal(SIGINT, heredoc_sigint_handler);
// 	signal(SIGQUIT, SIG_IGN);
// 	fd = open(file, O_WRONLY | O_CREAT, 0644);
// 	cleanup(6);
// 	if (data()->pipeline)
// 		data()->pipeline->next = cmd;
// 	else
// 		data()->pipeline = cmd;
// 	data()->chached_pwd = file;
// }

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
		_printfd(1,
					"\nminishell: warning: here-document\
 delimited by end-of-file (wanted `%s')\n",
					eof);
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
