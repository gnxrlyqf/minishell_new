/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchetoui <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 13:31:16 by mchetoui          #+#    #+#             */
/*   Updated: 2025/07/12 13:31:19 by mchetoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lexer.h>
#include <main.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>

t_cmd	*parse(char *input)
{
	t_shell	*shell;
	t_cmd	*pipeline;

	shell = data();
	shell->lexer = init_lexer(input);
	if (!shell->lexer)
	{
		_printfd(2, "Lexer init failed\n");
		free(input);
		return (NULL);
	}
	resolve_tokens(shell->lexer);
	if (check_syntax_errors(shell->lexer))
	{
		free_lexer(shell->lexer);
		free(input);
		return (NULL);
	}
	free(input);
	pipeline = create_pipeline(shell->lexer->tokens);
	free_lexer(shell->lexer);
	return (pipeline);
}

int	routine(void)
{
	char	*input;
	t_shell	*shell;

	shell = data();
	while (1)
	{
		shell->pipeline = NULL;
		input = readline("minishell> ");
		if (!input)
			return (_printfd(1, "\nexit\n"), shell->status);
		if (*input == '\0')
		{
			free(input);
			continue ;
		}
		add_history(input);
		if (parse(input))
			start(shell->pipeline);
		cleanup(1);
		shell->signal = 0;
	}
	return (0);
}

int	main(int ac, char **av, char **envp)
{
	int	status;

	(void)ac;
	(void)av;
	init_shell(envp);
	status = routine();
	cleanup(14);
	return (status);
}
