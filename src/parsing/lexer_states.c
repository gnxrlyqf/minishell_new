/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_states.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchetoui <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 12:55:36 by mchetoui          #+#    #+#             */
/*   Updated: 2025/07/12 12:55:37 by mchetoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lexer.h>

t_lexer	*set_state(t_lexer *lexer)
{
	char	current_char;

	current_char = *(lexer->offset);
	if (current_char == '\0')
		lexer->state = E_OF;
	if (lexer->context == Separator && !is_whitespace(current_char))
		lexer->in_heredoc_delim = 0;
	if (lexer->context == Separator)
	{
		if (is_whitespace(current_char))
			lexer->state = space;
		else if (current_char == '|')
			lexer->state = pi_pe;
		else if (current_char == '<')
			handle_redirect_in(lexer);
		else if (current_char == '>')
			handle_redirect_out(lexer);
	}
	else if (current_char == '\'' && lexer->context != Double_quoted)
		lexer->state = single_quote;
	else if (current_char == '"' && lexer->context != Quoted)
		lexer->state = double_quote;
	else if (current_char == '$' && lexer->context != Quoted
		&& is_valid_param_start(lexer->offset + 1))
	{
		if (lexer->in_heredoc_delim)
			lexer->state = literal;
		else
			lexer->state = param_here;
	}
	else if (lexer->context != Quoted && lexer->context != Double_quoted)
	{
		if (is_whitespace(current_char))
			lexer->state = space;
		else if (current_char == '<')
			handle_redirect_in(lexer);
		else if (current_char == '>')
			handle_redirect_out(lexer);
		else if (current_char == '|')
			lexer->state = pi_pe;
		else
			lexer->state = literal;
	}
	else
		lexer->state = literal;
	return (lexer);
}

void	set_context(t_lexer *lexer, char c)
{
	if (c == '\'' && lexer->context == Unquoted)
		lexer->context = Quoted;
	else if (c == '\'' && lexer->context == Quoted)
		lexer->context = Unquoted;
	else if (c == '"' && lexer->context == Unquoted)
		lexer->context = Double_quoted;
	else if (c == '"' && lexer->context == Double_quoted)
		lexer->context = Unquoted;
	else if (lexer->context == Separator)
	{
		if (c == '\'')
			lexer->context = Quoted;
		else if (c == '"')
			lexer->context = Double_quoted;
		else if (!is_seperator(c))
			lexer->context = Unquoted;
	}
	else if (lexer->context == Unquoted && is_seperator(c))
	{
		lexer->context = Separator;
		if (lexer->in_heredoc_delim)
			lexer->in_heredoc_delim = 0;
	}
}
