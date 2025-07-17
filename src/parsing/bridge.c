/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bridge.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchetoui <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 12:55:30 by mchetoui          #+#    #+#             */
/*   Updated: 2025/07/12 12:55:31 by mchetoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <_printfd.h>
#include <lexer.h>
#include <main.h>

void	count_words(t_list *list, int *args, int *redir)
{
	t_list	*curr;
	t_token	*tok;

	*args = 0;
	*redir = 0;
	curr = list;
	while (curr)
	{
		tok = (t_token *)curr->data;
		if (tok->type == Pipe || tok->type == End_of_file)
			break ;
		if (tok->type == Word)
			(*args)++;
		else if (tok->type <= 5 && tok->type >= 2)
		{
			(*args)--;
			(*redir)++;
		}
		curr = curr->next;
	}
}

t_cmd	*init_cmd(t_list *list)
{
	t_cmd	*cmd;
	int		argcount;
	int		redircount;
	int		i;

	count_words(list, &argcount, &redircount);
	cmd = malloc(sizeof(t_cmd));
	cmd->args = NULL;
	cmd->redir = NULL;
	if (argcount)
	{
		cmd->args = malloc(sizeof(char *) * (argcount + 1));
		i = 0;
		while (i <= argcount)
			cmd->args[i++] = NULL;
	}
	if (redircount)
	{
		cmd->redir = malloc(sizeof(t_redir) * redircount);
		i = 0;
		while (i < redircount)
			cmd->redir[i++].file = NULL;
	}
	cmd->next = NULL;
	return (cmd->argcount = argcount, cmd->redircount = redircount, cmd);
}

char	*check_heredoc(t_cmd *cmd, t_token *token)
{
	char	*eof;
	char	*path;
	int		expandable;

	expandable = 1;
	if (cmd->redir->type != Here_doc)
		return (_strdup(token->value));
	if (_strchr(token->value, '"') || _strchr(token->value, '\''))
	{
		expandable = 0;
		eof = quotes(token->value);
	}
	else
		eof = token->value;
	path = hdoc(cmd, eof, expandable);
	if (!expandable)
		free(eof);
	return (path);
}

t_cmd	*create_cmd(t_list **list, t_list *cpy)
{
	t_token	*tok;
	t_cmd	*cmd;
	int		i;

	cmd = init_cmd(cpy);
	i = 0;
	while (*list)
	{
		tok = (t_token *)(*list)->data;
		if (tok->type == Pipe || tok->type == End_of_file)
			break ;
		if (tok->type == Word)
			*(cmd->args++) = quotes_expand(tok->value, NULL);
		else if (tok->type >= 2)
		{
			cmd->redir[i].type = tok->type;
			*list = (*list)->next;
			cmd->redir[i].file = check_heredoc(cmd, (*list)->data);
			i++;
		}
		if (data()->signal == 130)
			break ;
		*list = (*list)->next;
	}
	return (cmd->args -= cmd->argcount, cmd);
}

t_cmd	*create_pipeline(t_list *list)
{
	t_list	*cpy;
	t_shell	*shell;
	t_cmd	*cmd;

	cpy = list;
	shell = data();
	shell->pipeline = create_cmd(&list, cpy);
	if (shell->signal == 130)
		return (NULL);
	cmd = shell->pipeline;
	while (list && ((t_token *)list->data)->type != End_of_file)
	{
		if (((t_token *)list->data)->type == Pipe)
		{
			list = list->next;
			continue ;
		}
		cpy = list;
		cmd->next = create_cmd(&list, cpy);
		if (shell->signal == 130)
			return (NULL);
		cmd = cmd->next;
	}
	return (shell->pipeline);
}
