#include <lexer.h>

void count_words(t_list *list, int *args, int *redir)
{
	t_list *curr;
	t_token *tok;

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

t_cmd *init_cmd(t_list *list)
{
	t_cmd *cmd;
	int argcount;
	int redircount;

	count_words(list, &argcount, &redircount);
	cmd = malloc(sizeof(t_cmd));
	cmd->args = malloc(sizeof(t_token) * argcount);
	cmd->redir = malloc(sizeof(t_redir) * redircount);
	cmd->argcount = argcount;
	cmd->redircount = redircount;
	cmd->next = NULL;
	return (cmd);
}

t_token check_heredoc(t_token_type type, t_token *token)
{
	char *eof;
	if (type != Here_doc)
		return (*token);
	eof = quotes(token->value);
	token->value = do_heredoc(eof, token->expendable);
	return (*token);
}

t_cmd *create_cmd(t_list **list, t_list *cpy)
{
	t_token *tok;
	t_cmd *cmd;
	t_list *curr;
	
	cmd = init_cmd(cpy);
	curr = *list;
	while (curr)
	{
		tok = (t_token *)curr->data;
		if (tok->type == Pipe || tok->type == End_of_file)
			break ;
		if (tok->type == Word)
			*(cmd->args++) = *tok;
		else if (tok->type >= 2)
		{
			cmd->redir->type = tok->type;
			curr = curr->next;
			(cmd->redir++)->file = check_heredoc((cmd->redir - 1)->type, curr->data);
		}
		curr = curr->next;
	}
	*list = curr;
	cmd->args -= cmd->argcount;
	cmd->redir -= cmd->redircount;
	return (cmd);
}

void print_cmd(t_cmd *cmd)
{
	int i;
	t_cmd *curr;

	curr = cmd;
	while (curr)
	{
		i = 0;
		while (i < curr->argcount)
			printf("arg: %s\n", curr->args[i++].value);
		i = 0;
		while (i < curr->redircount)
			printf("redir: %s - %s\n", token_type_to_str(curr->redir[i].type), curr->redir[i++].file.value);
		curr = curr->next;
	}
}

t_cmd *create_pipeline(t_list *list)
{
	t_list *cpy;
	t_cmd *head;
	t_cmd *cmd;

	cpy = list;
	head = create_cmd(&list, cpy);
	cmd = head;
	while (list && ((t_token *)list->data)->type != End_of_file)
	{
		if (((t_token *)list->data)->type == Pipe)
		{
			list = list->next;
			continue ;
		}
		cpy = list;
		cmd->next = create_cmd(&list, cpy);
		cmd = cmd->next;
	}
	return (head);
}