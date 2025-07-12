/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _printfd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchetoui <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 17:27:57 by mchetoui          #+#    #+#             */
/*   Updated: 2025/07/12 12:55:42 by mchetoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <_printfd.h>

int	_putstr(int fd, char *s)
{
	int	i;

	if (!s)
		return (write(fd, "(null)", 6));
	i = 0;
	while (s[i])
		write(fd, &s[i++], 1);
	return (i);
}

int	putn_b(int fd, long n, char *base)
{
	int	count;
	int	blen;

	blen = 10 + (6 * (base[10] != 0));
	count = write(fd, "-", n < 0);
	n = (n >= 0) * n - (n < 0) * n;
	if (n >= blen)
		count += putn_b(fd, n / blen, base);
	count += write(fd, &base[n % blen], 1);
	return (count);
}

int	putp(int fd, unsigned long n)
{
	int		count;
	char	*base;

	if (!n)
		return (write(fd, "(nil)", 5));
	base = "0123456789abcdef";
	count = write(fd, "0x", 2);
	if (n >= 16)
		count += putn_b(fd, n / 16, base);
	count += write(fd, &base[n % 16], 1);
	return (count);
}

int	handle_format(int fd, char c, va_list args)
{
	char	out;

	if (c == 'd' || c == 'i')
		return (putn_b(fd, (long)va_arg(args, int), "0123456789"));
	if (c == 'u')
		return (putn_b(fd, (long)va_arg(args, t_uint), "0123456789"));
	if (c == 's')
		return (_putstr(fd, va_arg(args, char *)));
	if (c == 'x')
		return (putn_b(fd, (t_uint)va_arg(args, int), "0123456789abcdef"));
	if (c == 'X')
		return (putn_b(fd, (t_uint)va_arg(args, int), "0123456789ABCDEF"));
	if (c == 'p')
		return (putp(fd, (uintptr_t)va_arg(args, void *)));
	if (c == 'c')
	{
		out = va_arg(args, int);
		return (write(fd, &out, 1));
	}
	if (c == '%')
		return (write(fd, "%", 1));
	return (-1);
}

int	_printfd(int fd, const char *str, ...)
{
	va_list	args;
	int		count;
	int		out;

	va_start(args, str);
	count = 0;
	if (!str)
		return (-1);
	while (*str)
	{
		if (*str == '%')
		{
			out = handle_format(fd, *(++str), args);
			if (out < 0)
			{
				count = -1;
				break ;
			}
			count += out;
		}
		else
			count += write(fd, str, 1);
		str++;
	}
	return (va_end(args), count);
}
