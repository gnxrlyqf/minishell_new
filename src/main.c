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

#include <main.h>
#include <lexer.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdio.h>
#include <stdlib.h>

t_shell	g_shell;

t_cmd	*parse(char *input)
{
	t_lexer	*lexer;

	lexer = init_lexer(input);
	if (!lexer)
	{
		fprintf(stderr, "Lexer init failed\n");
		free(input);
		return (NULL);
	}
	resolve_tokens(lexer);
	if (check_syntax_errors(lexer))
	{
		free_lexer(lexer);
		free(input);
		return (NULL);
	}
	g_shell.pipeline = create_pipeline(lexer->tokens);
	free_lexer(lexer);
	free(input);
	return (g_shell.pipeline);
}

int	routine(void)
{
	char	*input;

	setup_interactive_signals();
	while (1)
	{
		input = readline("minishell> ");
		if (!input)
		{
			write(1, "\nexit\n", 6);
			return (g_shell.status);
		}
		if (*input == '\0')
		{
			free(input);
			continue ;
		}
		add_history(input);
		parse(input);
		start(g_shell.pipeline);
		free_pipeline(g_shell.pipeline);
		setup_interactive_signals();
		// cleanup(1);
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
	cleanup(6);
	return (status);
}
