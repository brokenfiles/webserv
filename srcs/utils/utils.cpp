//
// Created by Timothe Lecouvreur on 07/01/2021.
//
#include <stdlib.h>

static int      ft_slen(const char *str)
{
	int     i;

	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}

char            *ft_strdup(const char *str)
{
	int             i;
	char    *res;

	i = 0;
	if (!(res = (char*)malloc(sizeof(char) * (ft_slen(str) + 1))))
		return (NULL);
	while (str[i] != '\0')
	{
		res[i] = str[i];
		i++;
	}
	res[i] = '\0';
	return (res);
}

int             get_nb_size(int nb)
{
	int     i;

	i = 0;
	if (nb < 0)
	{
		i++;
		nb *= -1;
	}
	if (nb == 0)
		return (1);
	while (nb)
	{
		i++;
		nb /= 10;
	}
	return (i);
}

char    *ft_itoa(int nb)
{
	int             i;
	char    *res;

	i = get_nb_size(nb);
	if (nb == -2147483648)
		return (ft_strdup("-2147483648"));
	if (nb == 0)
		return (ft_strdup("0"));
	if (!(res = (char*)malloc(sizeof(char) * (i + 1))))
		return (NULL);
	if (nb < 0)
	{
		res[0] = '-';
		nb *= -1;
	}
	i--;
	res[i + 1] = '\0';
	while (nb)
	{
		res[i] = ((nb % 10) + '0');
		i--;
		nb /= 10;
	}
	return (res);
}