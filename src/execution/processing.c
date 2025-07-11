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

char	*quotes_expand(char *str)
{
	t_list	*list;
	char	*cpy;
	char	*ret;
	int		c;

	list = NULL;
	c = -1;
	cpy = str;
	while (*cpy)
	{
		if (*cpy == '$' && c != '\'' && *(cpy + 1) > 32)
			cpy += fill_var(cpy + 1, &list);
		if ((*cpy == '\'' || *cpy == '"') && c == -1)
			c = *cpy;
		else if (*cpy == c)
			c = -1;
		else
			add_node(&list, cpy);
		if (!*cpy)
			break ;
		cpy++;
	}
	ret = make_str(list);
	free_list(&list);
	return (ret);
}

char	**extract_args(t_token *tokens, int size)
{
	int		i;
	char	**arr;

	arr = malloc(sizeof(char *) * (size + 1));
	arr[size] = NULL;
	i = -1;
	while (++i < size)
		arr[i] = quotes_expand(tokens[i].value);
	return (arr);
}

char	*do_heredoc(char *eof, int expand)
{
	int		fd;
	char	*file;
	char	*line;

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
