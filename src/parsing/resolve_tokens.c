/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resolve_tokens.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchetoui <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 12:55:39 by mchetoui          #+#    #+#             */
/*   Updated: 2025/07/12 12:55:39 by mchetoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lexer.h>
#include <main.h>

t_token_type	get_token_type(t_state state)
{
	if (state == pi_pe)
		return (Pipe);
	if (state == redirect_in)
		return (Redirect_In);
	if (state == redirect_out)
		return (Redirect_Out);
	if (state == append)
		return (Append);
	if (state == heredoc)
		return (Here_doc);
	if (state == E_OF)
		return (End_of_file);
	return (Word);
}

t_quote_type	get_quote_type(t_context context)
{
	if (context == Quoted)
		return (Single_quotes);
	if (context == Double_quoted)
		return (Double_quotes);
	return (No_quotes);
}

int	is_operator_state(t_state state)
{
	return (state == pi_pe || state == redirect_in || state == redirect_out
		|| state == append || state == heredoc);
}

int	is_data_state(t_state state)
{
	return (state == literal || state == param_here || state == single_quote
		|| state == double_quote);
}

void	create_operator_token(t_lexer *lexer)
{
	if (lexer->state == heredoc)
	{
		append_token(lexer, create_token("<<", Here_doc, Not_expendable));
		lexer->offset += 2;
		lexer->in_heredoc_delim = 1;
	}
	else if (lexer->state == append)
	{
		append_token(lexer, create_token(">>", Append, Not_expendable));
		lexer->offset += 2;
	}
	else if (lexer->state == redirect_in)
	{
		append_token(lexer, create_token("<", Redirect_In, Not_expendable));
		lexer->offset++;
	}
	else if (lexer->state == redirect_out)
	{
		append_token(lexer, create_token(">", Redirect_Out, Not_expendable));
		lexer->offset++;
	}
	else if (lexer->state == pi_pe)
	{
		append_token(lexer, create_token("|", Pipe, Not_expendable));
		lexer->offset++;
	}
}

static void	finalize_current_token(t_lexer *lexer, char **start,
		t_expendable *expend)
{
	char	*chunk;

	if (!*start)
		return ;
	chunk = ft_substr(*start, 0, lexer->offset - *start);
	if (!chunk)
		return ;
	append_token(lexer, create_token(chunk, Word, *expend));
	free(chunk);
	*start = NULL;
	*expend = Not_expendable;
}

void	resolve_tokens(t_lexer *lexer)
{
	t_expendable	expend;
	char			*start;

	expend = Not_expendable;
	start = NULL;
	while (*(lexer->offset))
	{
		set_context(lexer, *(lexer->offset));
		set_state(lexer);
		if (lexer->state == space && !start)
		{
			lexer->offset++;
			continue ;
		}
		if (lexer->state == param_here)
			expend = Expendable;
		if (is_operator_state(lexer->state))
		{
			finalize_current_token(lexer, &start, &expend);
			create_operator_token(lexer);
			continue ;
		}
		if (!start)
			start = lexer->offset;
		if (lexer->context == Separator && lexer->state == space)
			finalize_current_token(lexer, &start, &expend);
		lexer->offset++;
	}
	finalize_current_token(lexer, &start, &expend);
	append_token(lexer, create_token("", End_of_file, Not_expendable));
}

#define MAX_HEREDOCS 16

static int	is_redirection(t_token_type type)
{
	return (type == Redirect_In || type == Redirect_Out
		|| type == Append || type == Here_doc);
}

static int	is_operator(t_token_type type)
{
	return (type == Pipe || is_redirection(type));
}

static int	unclosed_quote_error(t_context ctx)
{
	if (ctx == Quoted)
		printf("minishell: syntax error: unexpected newline while looking for matching '\''\n");
	else if (ctx == Double_quoted)
		printf("minishell: syntax error: unexpected newline while looking for matching '\"'\n");
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
		if (curr->type == Pipe && next && (next->type == Pipe || next->type == End_of_file))
			return (throw_err(INV_TOKEN, curr->value), 1);
		if (!prev && curr->type == Pipe)
			return (throw_err(INV_TOKEN, curr->value), 1);
		if (is_redirection(curr->type)
			&& (!next || is_operator(next->type) || next->type == End_of_file))
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
