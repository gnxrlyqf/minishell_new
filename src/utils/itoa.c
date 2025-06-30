#include <main.h>

static int    int_length(long n)
{
    int    i;

    i = 0;
    if (n <= 0)
        i++;
    if (n < 0)
        n *= -1;
    while (n > 0)
    {
        n = n / 10;
        i++;
    }
    return (i);
}

static char    *convert(char *n_str, int len, long num)
{
    int    i;

    i = 0;
    if (num < 0)
    {
        n_str[0] = '-';
        num *= -1;
        i = 1;
    }
    n_str[len] = '\0';
    len--;
    while (i <= len)
    {
        n_str[len--] = (num % 10) + '0';
        num /= 10;
    }
    return (n_str);
}

char	*_itoa(int n)
{
    char    *n_str;
    int        i;

    if (n == 0)
    {
        i = 0;
        n_str = malloc(2);
        if (!n_str)
            return (0);
        n_str[i] = '0';
        i++;
        n_str[i] = 0;
        return (n_str);
    }
    n_str = malloc(int_length(n) + 1);
    if (!n_str)
        return (0);
    n_str = convert(n_str, int_length(n), n);
    return (n_str);
}