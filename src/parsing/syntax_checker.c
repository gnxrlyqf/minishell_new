#include <lexer.h>
#include <main.h>

static int	is_operator(t_token_type type)
{
	return (type == Pipe || is_redirection(type));
}

static int	unclosed_quote_error(t_context ctx)
{
	if (ctx == Quoted)
		printf("minishell: syntax error: unexpected newline \
            while looking for matching '\''\n");
	else if (ctx == Double_quoted)
		printf("minishell: syntax error: unexpected newline \
            while looking for matching '\"'\n");
	return (ctx == Quoted || ctx == Double_quoted);
}

static int	check_token_sequence(t_list *lst)
{
	t_token	*curr;
	t_token	*next;
	t_token	*prev;

	while (lst)
	{
		curr = lst->data;
		next = NULL;
		prev = NULL;
		if (lst->next)
			next = lst->next->data;
		if (lst->prev)
			prev = lst->prev->data;
		if (curr->type == Pipe && next && (next->type == Pipe
				|| next->type == End_of_file))
			return (throw_err(INV_TOKEN, curr->value), 1);
		if (!prev && curr->type == Pipe)
			return (throw_err(INV_TOKEN, curr->value), 1);
		if (is_redirection(curr->type) && (!next || is_operator(next->type)
				|| next->type == End_of_file))
			return (throw_err(INV_TOKEN, next->value), 1);
		lst = lst->next;
	}
	return (0);
}

int	check_syntax_errors(t_lexer *lexer)
{
	if (unclosed_quote_error(lexer->context))
		return (1);
	if (check_token_sequence(lexer->tokens))
		return (1);
	return (0);
}
