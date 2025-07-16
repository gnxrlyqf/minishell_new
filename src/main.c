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

void	parse(char *input)
{
	t_lexer	*lexer;
	t_cmd *pipeline;

	lexer = init_lexer(input);
	if (!lexer)
	{
		fprintf(stderr, "Lexer init failed\n");
		free(input);
		return ;
	}
	resolve_tokens(lexer);
	if (check_syntax_errors(lexer))
	{
		free_lexer(lexer);
		free(input);
		return ;
	}
	pipeline = create_pipeline(lexer->tokens);
	free_lexer(lexer);
	free(input);
	data()->pipeline = pipeline;
}

int	routine(void)
{
	char	*input;
	t_shell *shell;

	shell = data();
	while (1)
	{
		input = readline("minishell> ");
		if (!input)
		{
			write(1, "\nexit\n", 6);
			return (shell->status);
		}
		if (*input == '\0')
		{
			free(input);
			continue ;
		}
		add_history(input);
		parse(input);
		if (!shell->pipeline)
			continue ;
		start(shell->pipeline);
		cleanup(1);
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
