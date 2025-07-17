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

static void	create_heredoc_token(t_lexer *lexer)
{
	append_token(lexer, create_token("<<", Here_doc, Not_expendable));
	lexer->offset += 2;
	lexer->in_heredoc_delim = 1;
}

void	create_operator_token(t_lexer *lexer)
{
	if (lexer->state == heredoc)
		create_heredoc_token(lexer);
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

static void	handle_token(t_lexer *lexer, char **start, t_expendable *expend)
{
	if (lexer->state == space && !*start)
	{
		lexer->offset++;
		return ;
	}
	if (lexer->state == param_here)
		*expend = Expendable;
	if (is_operator_state(lexer->state))
	{
		finalize_current_token(lexer, start, expend);
		create_operator_token(lexer);
		return ;
	}
	if (!*start)
		*start = lexer->offset;
	if (lexer->context == Separator && lexer->state == space)
		finalize_current_token(lexer, start, expend);
	lexer->offset++;
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
		handle_token(lexer, &start, &expend);
	}
	finalize_current_token(lexer, &start, &expend);
	append_token(lexer, create_token("", End_of_file, Not_expendable));
}
