/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchetoui <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 18:15:51 by mchetoui          #+#    #+#             */
/*   Updated: 2025/07/17 18:15:52 by mchetoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lexer.h>
#include <main.h>
#include <_printfd.h>

static int	is_operator(t_token_type type)
{
	return (type == Pipe || is_redirection(type));
}

static int	unclosed_quote_error(t_context ctx)
{
	if (ctx == Quoted)
		_printfd(2, "minishell: syntax error: unexpected newline \
            while looking for matching '\''\n");
	else if (ctx == Double_quoted)
		_printfd(2, "minishell: syntax error: unexpected newline \
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

static int	check_heredoc_limit(t_list *lst)
{
	t_token	*tok;
	int		count;

	count = 0;
	while (lst)
	{
		tok = lst->data;
		if (tok->type == Here_doc)
			count++;
		if (count > MAX_HEREDOCS)
		{
			_printfd(2, "minishell: maximum here-document count exceeded\n");
			return (1);
		}
		lst = lst->next;
	}
	return (0);
}

int	check_syntax_errors(t_lexer *lexer)
{
	if (check_heredoc_limit(lexer->tokens))
		return (1);
	if (unclosed_quote_error(lexer->context))
		return (1);
	if (check_token_sequence(lexer->tokens))
		return (1);
	return (0);
}
