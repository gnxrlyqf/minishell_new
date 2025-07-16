/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchetoui <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 12:55:40 by mchetoui          #+#    #+#             */
/*   Updated: 2025/07/12 12:55:41 by mchetoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lexer.h>
#include <main.h>

void	sigint_handler(int sig)
{
    (void)sig;
    data()->status = 130;
    write(1, "\n", 1);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

void heredoc_sigint_handler(int sig)
{
    t_shell *shell;

    (void)sig;
    shell = data();
    write(1, "\n", 1);
    // cleanup(13);
    exit(130);
}

