#include <main.h>

extern t_shell g_shell;

void	ft_sigint_handler(int sig)
{
	(void)sig;
	if (g_shell.in_execution)
		// Execution: let child get SIGINT, print newline
		write(1, "\n", 1);
	else if (g_shell.heredoc_sigint)
	{
		// Heredoc: just mark interrupted
		write(1, "\n", 1);
		g_shell.heredoc_sigint = true;
	}
	else
	{
		// Prompt: reset prompt
		write(STDOUT_FILENO, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

void	setup_prompt_signals(void)
{
	struct termios term;

	tcgetattr(STDIN_FILENO, &term);
	// term.c_lflag &= ~ECHOCTL;
	term.c_lflag |= ECHOCTL; // Enable ECHOCTL to show ^C for SIGINT
	tcsetattr(STDIN_FILENO, TCSANOW, &term);

	g_shell.in_execution = false;
	g_shell.heredoc_sigint = false;

	signal(SIGINT, ft_sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	setup_exec_signals(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	g_shell.in_execution = true;
}

void	reset_signals_in_child(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	setup_heredoc_signals(void)
{
	signal(SIGINT, ft_sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	g_shell.heredoc_sigint = false;
}
