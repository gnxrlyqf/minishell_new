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

void	foo(int sig)
{
	(void)sig;
	g_shell.status = 130;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	init_shell(char **envp)
{
	g_shell.env = init_env(envp);
	g_shell.status = 0;
	g_shell.chached_pwd = NULL;
	g_shell.sig = 0;
	// tcgetattr(STDIN_FILENO, &g_shell.orig_termios);
	signal(SIGINT, foo);
	signal(SIGQUIT, SIG_IGN);
}

void	cleanup(int n)
{
	if (1 & n)
		free_pipeline(g_shell.pipeline);
	if (2 & n)
		free_env(g_shell.env);
	if (4 & n)
		free(g_shell.chached_pwd);
}
