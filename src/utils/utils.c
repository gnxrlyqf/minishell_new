/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchetoui <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 12:55:49 by mchetoui          #+#    #+#             */
/*   Updated: 2025/07/12 12:55:49 by mchetoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <main.h>

char	*max_str(char *a, char *b)
{
	if (a > b)
		return (a);
	return (b);
}

t_shell	*data(void)
{
	static t_shell	*shell;

	if (!shell)
	{
		shell = malloc(sizeof(t_shell));
		if (!shell)
			throw_err(SYSCALL_FAIL, "malloc");
	}
	return (shell);
}

void	init_shell(char **envp)
{
	t_shell	*shell;

	shell = data();
	shell->env = init_env(envp);
	shell->signal = 0;
	shell->status = 0;
	shell->chached_pwd = NULL;
	shell->pipeline = NULL;
	tcgetattr(STDIN_FILENO, &data()->orig_termios);
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	cleanup(int n)
{
	t_shell	*shell;

	shell = data();
	if (1 & n)
		free_pipeline(shell->pipeline);
	if (2 & n)
		free_env(shell->env);
	if (4 & n)
		free(shell->chached_pwd);
	if (8 & n)
		free(shell);
}
