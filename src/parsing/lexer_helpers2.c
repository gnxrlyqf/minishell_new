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

int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

int	is_quote(char c)
{
	return (c == '\'' || c == '\"');
}

int	is_redirect(char c)
{
	return (c == '<' || c == '>');
}

int	is_seperator(char c)
{
	return (is_whitespace(c) || is_redirect(c) || c == '|' || c == '\0');
}

int	is_valid_param_start(char *str)
{
	if (!str || !*str)
		return (0);
	if (*str == '?')
		return (1);
	return (ft_isalpha(*str) || *str == '_');
}
