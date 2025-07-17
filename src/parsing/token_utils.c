/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchetoui <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 18:15:38 by mchetoui          #+#    #+#             */
/*   Updated: 2025/07/17 18:15:40 by mchetoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lexer.h>

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

int	is_redirection(t_token_type type)
{
	return (type == Redirect_In || type == Redirect_Out
		|| type == Append || type == Here_doc);
}
