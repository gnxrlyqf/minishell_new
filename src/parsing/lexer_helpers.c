/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchetoui <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 12:55:32 by mchetoui          #+#    #+#             */
/*   Updated: 2025/07/12 12:55:33 by mchetoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lexer.h>

int	is_valid_param_char(char c)
{
	if (c == '?')
		return (1);
	return (ft_isalnum(c) || c == '_');
}

int	contains_parameter(char *value)
{
	char	*dollar_sign;

	dollar_sign = ft_strchr(value, '$');
	while (dollar_sign)
	{
		if (is_valid_param_start(dollar_sign + 1))
			return (1);
		dollar_sign = ft_strchr(dollar_sign + 1, '$');
	}
	return (0);
}

void	handle_redirect_in(t_lexer *lexer)
{
	if (*(lexer->offset + 1) == '<')
		lexer->state = heredoc;
	else
		lexer->state = redirect_in;
}

void	handle_redirect_out(t_lexer *lexer)
{
	if (*(lexer->offset + 1) == '>')
		lexer->state = append;
	else
		lexer->state = redirect_out;
}

void	skip_space(t_lexer *lexer)
{
	if (lexer->state == space)
		lexer->offset++;
}
