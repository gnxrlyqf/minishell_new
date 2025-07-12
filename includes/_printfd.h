/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _printfd.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchetoui <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 17:30:04 by mchetoui          #+#    #+#             */
/*   Updated: 2024/11/18 17:30:22 by mchetoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _PRINTFD_H
# define _PRINTFD_H

# include <stdarg.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdint.h>

typedef unsigned int	t_uint;

int		_printfd(int fd, const char *str, ...);
int		_putstr(int fd, char *s);
int		putn_b(int fd, long n, char *base);
int		handle_format(int fd, char c, va_list args);
int		putp(int fd, unsigned long n);

#endif
